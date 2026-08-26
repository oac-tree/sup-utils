======================
Command-Line Utilities
======================

The ``sup/cli`` package provides tools for parsing and handling command-line arguments.

**Key Features**:

  - Parsing command-line arguments into flags, parameters, and positional values.
  - Required options, default values, and conversion of a value to the requested type.
  - Automatically generated usage string, and a help option.
  - Errors that name the option and the value at fault.

**Main Components**:

  1. ``CommandLineParser``: registers the expected options, parses the command line, and gives
     access to the values found.
  2. ``CommandLineOption``: a single expected option, i.e. its names, description, default value,
     and whether it is required.
  3. ``ParseOrReport``: parses on behalf of ``main``, reports the outcome, and yields the value
     ``main`` should return.
  4. Exceptions: ``InvalidOptionException``, ``InvalidValueException``, ``MissingValueException``.

**Option Types**:

  1. ``Flags``: arguments beginning with a dash, e.g. ``-v``, ``--verbose``. Represent a boolean.
  2. ``Parameters``: a name followed by a value, e.g. ``--font 24`` or ``--font=24``.
  3. ``Positional``: free standing values, e.g. ``config.xml``. Their names are used in the help
     text only, and are matched to the values in the order the options were added.

**Example**:

.. code-block:: c++

  sup::cli::CommandLineParser parser;

  parser.AddHelpOption();
  parser.AddOption({"-f", "--file"}, "File to process").SetParameter(true).SetRequired(true);
  parser.AddOption({"--font"}, "Font size").SetParameter(true).SetDefaultValue("12");
  parser.AddOption({"-v", "--verbose"}, "Log to standard output");
  parser.AddPositionalOption("<target>", "Where to write the result").SetRequired(true);

  // Prints the usage string when the help was requested, the reason when the command line doesn't
  // add up, and yields the value main should return. An empty result means "carry on".
  const auto exit_code = sup::cli::ParseOrReport(parser, argc, argv);
  if (exit_code.has_value())
  {
    return *exit_code;
  }

  const auto verbose = parser.IsSet("--verbose");
  const auto filename = parser.GetValue<std::string>("--file");
  const auto font_size = parser.GetValue<int>("--font");
  const auto target = parser.GetPositionalValue<std::string>(0);

**Accessing the values**:

  - ``IsSet``: whether the option was given in the command line.
  - ``GetValue<T>``: the value converted to ``T``. The whole text has to convert, so ``24k`` asked
    for as an ``int`` is an error rather than ``24``. The default value is used when the option was
    not given.
  - ``GetValue<T>(name, fallback)``: the same, with ``fallback`` standing in for an option that was
    not given and has no default value.
  - ``GetValueInRange<T>``: the value, checked against the given bounds.
  - ``FindValue``: the text of the value, or an empty ``std::optional`` if the option was not
    given. It tells that case apart from a value that was given as an empty string.
  - ``GetPositionalOptionCount`` and ``GetPositionalValue<T>``: the free standing values found.

A complete application is in ``test/cli-example`` of this package.
