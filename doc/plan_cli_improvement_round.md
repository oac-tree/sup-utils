# Plan: sup-cli improvement round

Working document for the `sup/cli` review on branch `COA-2255`, target version 1.11.0.
**Temporary — delete before the branch is merged.** Sphinx only reads `.rst`, so this file is
inert in the documentation build.

## 1. Where we are

Committed in this round:

| Commit | Change |
| --- | --- |
| `d76a9df` | `CommandLineParser::IsSet` is `const` (+ `QueryConstParser` test) |
| `bdb2a07` | `GetOption` split into a `const`/non-`const` pair delegating to `CommandLineParserImpl::FindOption` (+ `GetOptionFromConstParser` test) |
| `76f72d3` | Option and positional values keep whitespace: `GetValueStream` no longer re-splits an already-split token, plus `std::string` specializations of `GetValue`/`GetPositionalValue` (+ two tests) |
| `70163a1`, `2c64091` | Project-wide formatting, `.clang-format`, `.clang-tidy`, comment style |

82 unit tests pass. Both out-of-tree consumers of `GetOption` still compile
(`psps-protocol`, `psps-lms`).

The two API fixes are ABI breaks (mangled names gained `K`). `LIBSOVERSION` is
`MAJOR.MINOR`, so the 1.11.0 bump already covers the whole round — no further action.

## 2. Behaviour baseline

Measured against the current code, not inferred. Useful when writing the steps below, and as the
regression reference.

Fixed in this round:

| Command line | Was | Now |
| --- | --- | --- |
| `--file "/path/with space/a.xml"` | `/path/with` | full value |
| `--msg "hello world"` | `hello` | `hello world` |
| positional `"/path/with space/a.xml"` | `/path/with` | full value |
| default `"a default with spaces"` | `a` | full default |

Still open:

| Command line | Result | Item |
| --- | --- | --- |
| `--size 64k` → `GetValue<int>` | `64` | step 8 |
| `--n -1` → `GetValue<unsigned>` | `4294967295` | step 8 |
| `IsSet("--typo")` on an unregistered name | `false`, same as "not given" | step 10 |
| `GetValue` on an option never given | `std::runtime_error("Can't parse the value")` — names neither the option nor the real cause | step 7 |
| `SetDefaultValue("")` | indistinguishable from "no default"; `GetValue` throws | step 9 |
| `--msg ""` (explicitly empty) | treated as not given | step 9 |
| option name string longer than 30 chars | help line silently truncated: `-r, --receive-buffer-size <byt` | step 4 |
| two `AddOption` calls with the same name | second silently dead | step 10 |
| `GetPositionalOptionCount()` before `Parse` | `-1`, and the type is signed | step 12 |
| `Parse` returns `false` | help, missing required option and missing parameter value are indistinguishable | step 11 |
| `--file "-weird name"` | `--file` degrades to a flag, `Parse` fails, token dropped entirely | step 14 |
| `--file a.xml -- -target.xml` | `--` is not an end-of-options terminator; both tokens vanish | step 14 |
| `--file target.xml` with `<target>` positional registered | value eats the positional, count 0 | step 14 |

Argument boundaries are `argv` elements, not whitespace — the shell has already split the command
line, and argh consumes exactly one element per parameter. That is why step 3 was safe: the old
`operator>>` was re-splitting a token the shell had already delimited. Order of options and
positionals does not matter, and the equals form (`--file=a b`) works.

## 3. Remaining steps

Ordered so each one is independently committable. Cheap and invisible first, additive API next,
behaviour changes last.

### Step 4 — Help text truncation *(display only, no API change)*

`GetOptionUsageString` ([command_line_utils.cpp:107](../src/lib/sup/cli/command_line_utils.cpp))
uses `result.resize(kDesiredOptionStringLength, ' ')`, which *shrinks* a longer name string.
Pad when shorter; when longer, append a separator or wrap the description to the next line. Also
stop emitting trailing spaces for options with no description.

Test: an option whose name string exceeds 30 characters keeps its full name; an option with no
description produces no trailing whitespace.

### Step 5 — Keep clang-tidy usable *(build only)*

The new `.clang-tidy` produces ~60 warnings from the vendored
[argh.h](../src/lib/sup/cli/argh.h) alone, which buries the ~15 real ones from our own files.
Exclude the third-party header — a `.clang-tidy` with `Checks: '-*'` beside it, a `thirdparty/`
subdirectory, or a `HeaderFilterRegex` that excludes it. Do this before step 6 so the remaining
steps can be checked.

Our own warnings worth acting on (the rest is noise):

- `hicpp-special-member-functions` on `CommandLineParser` → step 6
- `readability-const-return-type` on `GetFilteredOptions` → step 13
- `performance-unnecessary-value-param` in the `MergeWithNewLine` lambda → step 13
- `bugprone-easily-swappable-parameters` on `GetUsageString(app_name, header, …, footer)` and
  `SetDescription(header, footer)` → step 13
- `modernize-concat-nested-namespaces` across all headers → step 13
- `readability-use-anyofallof` in `IsValidParsing`, several `misc-const-correctness` → step 13

### Step 6 — Make the parser movable *(additive)*

Verified: `is_copy_constructible`, `is_move_constructible` and `is_move_assignable` are all
**false**. The user-declared destructor (needed for the pimpl) suppresses the implicit moves, and
`unique_ptr` deletes the copies. A factory function cannot return a configured parser by value,
which is the natural way to share option sets between applications — exactly what
`sdn2::tools::RegisterCommonOptions` works around by taking the parser by reference.

Declare and define `CommandLineParser(CommandLineParser&&) noexcept` and the move assignment in
the `.cpp`; leave copying deleted. Test with `static_assert` on the traits.

### Step 7 — A real exception type *(additive, enables 8–11)*

Add `sup/cli/exceptions.h` mirroring [sup/xml/exceptions.h](../src/lib/sup/xml/exceptions.h)
(`MessageException` base, then `ParseException` / `InvalidOptionException`). Messages must name the
option and the offending value. Replace the two `std::runtime_error` throws in
`command_line_parser.cpp`, and distinguish the three cases currently sharing one message: unknown
option, option not given, no default available.

Keep deriving from `std::exception`: `sdn2`'s `main` and the other consumers already catch that, so
nothing downstream changes. Install the new header.

### Step 8 — Strict `GetValue<T>` *(behaviour change)*

Convert the *whole* text or throw. Reject trailing text (`64k`), leading whitespace, an empty
value, and a negative literal read into an unsigned type. The implementation to lift is
`sdn2`'s `IsUnusableNumericText` plus a full-consumption check.

Route the primary template through a private `std::string GetValueAsText(name)` and do the
conversion in one place; that also removes `<sstream>` and the two `…Stream` methods from the
public header. Keep the `std::string` specializations from step 3 short-circuiting.

Risk: an application today relying on `GetValue<int>("--size")` quietly yielding `64` for `64k`
starts throwing. That is the point, but it is the one step that can change a working command line
into a failing one — worth a ChangeLog note.

Also add `GetValueInRange<T>(name, lowest, highest)`, or per-option bounds on `CommandLineOption`
validated during `Parse` so the error arrives with the usage message instead of at the first
`GetValue`.

### Step 9 — Defaults and optional values *(behaviour change)*

Replace the "empty default means no default" convention with an explicit `m_has_default` flag on
`CommandLineOption`, so `SetDefaultValue("")` is representable. Then make the presence test in
`GetValueStream` independent of emptiness, so `--msg ""` counts as given.

Add `std::optional<std::string> FindValue(name)` and `GetValue<T>(name, fallback)`. These two are
what let the consumer drop its wrappers — see section 4.

### Step 10 — Asking about an unknown option is a programming error *(behaviour change)*

`IsSet("--typo")` returning `false` makes a misspelled name in application code
indistinguishable from an option the user did not pass. Throw instead. Same for `AddOption` with a
name already registered — the second registration is currently dead code.

Keep `GetOption` returning `nullptr`, since `psps-lms` uses exactly that as an
"already configured?" probe ([util.cpp:32](../../psps-lms/test/app/component-file-fixup/util.cpp)).

### Step 11 — Tell the caller *why* parsing failed *(additive)*

`Parse` collapses help-requested, missing-required-option and missing-parameter-value into one
`false` ([command_line_parser.cpp:138](../src/lib/sup/cli/command_line_parser.cpp)). An application
can only dump the whole usage block; it can never say which option was wrong. This is the biggest
usability gap left.

Add `GetParseError()` (message, or an enum plus the offending option name) and a separate
`IsHelpRequested()`. `AddHelpOption` should record which option is the help option instead of
`Parse` hardcoding `IsSet("--help")` — a help option registered under any other spelling is
currently not detected.

### Step 12 — Positional options *(mixed)*

- `GetPositionalOptionCount()` returns `int32` and `-1` before parsing, so every call site is
  either wrong or awkward — the project's own example writes
  `for (size_t i = 0; i < parser.GetPositionalOptionCount(); ++i)`, a signed/unsigned comparison
  that also misbehaves on `-1`. Move to `std::size_t` plus an explicit `HasParsed()`.
- `AddPositionalOption` returns `void` while `AddOption` returns a reference, and a positional
  cannot be marked required.
- Registered positionals feed the help text only; nothing checks that the expected number arrived.
  Validating the count during `Parse` is what turns the step-14 ambiguity from a silent misparse
  into a message.
- `GetPositionalValueStream` rebuilds the whole vector per access and casts `index` to `int32`
  before comparing.

### Step 13 — Small cleanups *(no behaviour change)*

- [base_types.h](../src/lib/sup/cli/base_types.h): `using int32 = signed int` is an installed
  public alias reimplementing `std::int32_t`. Use `<cstdint>`, and plain `int` for `Parse`'s `argc`
  so it matches `main`.
- `CommandLineOption` getters return `std::string`/`std::vector<std::string>` by value; `GetOption`
  copies a vector of strings per option per lookup, and `IsSet` does it again. Return `const&`.
- `CommandLineParser::SetDescription(header, footer)` collides in name with
  `CommandLineOption::SetDescription` and does something different; clang-tidy also flags the two
  swappable parameters. Rename, or split into two setters.
- `GetFilteredOptions` returns a top-level-`const` vector by value.
- `#endif` comments in `command_line_parser.h` and `command_line_utils.h` both say
  `SUP_CLI_COMMAND_LINE_OPTION_H_`.
- The `@code` block in [command_line_parser.h](../src/lib/sup/cli/command_line_parser.h) and
  [doc/cli.rst](cli.rst) do not compile: both use `AddOption("--file")->SetParameter(true)` and
  `parser.parse(...)`, but `AddOption` takes a vector and returns a reference, and the method is
  `Parse`. `GetAvailableOptionsSummaryString`'s doc comment promises
  `"[-h, --help] [--font=<value>] …"` while the code returns `"[options] <positional>"`.
- Record which upstream commit [argh.h](../src/lib/sup/cli/argh.h) came from; it is vendored
  (Adi Shavit, MIT) and locally modified with `#pragma GCC diagnostic` edits.
- Apply the clang-tidy items listed in step 5.

### Step 14 — Argument grammar limits *(decide, then maybe won't-fix)*

Three pre-existing gaps, all inherited from argh's grammar rather than from our wrapper:

1. A value starting with `-` cannot be passed in the space-separated form: `--file "-weird name"`
   makes `--file` a flag, fails `Parse`, and drops the token so it does not even reach the
   positionals. The equals form `--file=-weird name` works.
2. `--` is not honoured as an end-of-options terminator; it and everything after it disappear.
   Anything expecting POSIX behaviour will be surprised.
3. A trailing parameter option eats a positional: `--file target.xml` with `<target>` registered
   gives `file=target.xml` and zero positionals. Inherent to "a named value followed by a
   non-option" — the parser cannot know which was meant.

(1) and (2) need changes inside the vendored parser or a pre-pass over `argv`, so the honest
options are "document them" or "replace argh". (3) cannot be fixed at all, only detected — which
is the argument for validating the positional count in step 12. Recommend: document all three in
`doc/cli.rst`, implement the step 12 check, and leave argh alone.

## 4. What this gives the consumer

[sdn2-core](../../sdn2-core/src/lib/sdn2-tools/sdn2/tools/perf/cli_helper.h) wrote a helper module
because the library was missing the pieces above. After steps 7–9 it collapses:

| sdn2 helper | Root cause | Replacement |
| --- | --- | --- |
| `ParseInteger`, `ParseIntegerInRange`, `ParsePositiveDouble` | `GetValue<T>` stops at the first bad character | strict `GetValue<T>` / `GetValueInRange<T>` (step 8) |
| `ThrowInvalidOption` | one untyped `runtime_error` naming nothing | `sup::cli::InvalidOptionException` (step 7) |
| `GetGivenValue` = `IsSet(x) ? GetValue(x) : ""` | `GetValue` throws for an option with no default | `FindValue` / `GetValue<T>(name, fallback)` (step 9) |
| `GetValueOrDefault` (in `perf_options.cpp`) | nothing — a pure pass-through wrapper | disappears |
| `IsWritableDirectory` | not a CLI concern | stays out of sup-cli; belongs next to `sup/platform/host.cpp` |
| `CreateNetworkAddress(parser)` etc. taking a non-`const` parser | `IsSet` was non-`const` | **done** (`d76a9df`) — these can take `const&` now |

`cli_helper` then holds only `IsWritableDirectory`, and the
`parser.IsSet(x) ? parser.GetValue<…>(x) : …` pattern repeated about ten times in
`perf_options.cpp` becomes one call each. Coordinate the sdn2 change after step 9 lands, in a
separate branch there.

## 5. Out of scope

- Replacing argh with another parser, or vendoring a newer version.
- Subcommands (`git`-style `verb --opt`).
- Option groups, mutually exclusive options, repeated options collecting into a list. argh keeps a
  multimap and could support the last one, but nothing asks for it yet.
- Environment variable or config file fallback for option values.
