# Performance benchmarks

Google Benchmark measurements of `TreeData` build-up, copy, move and comparison, and of XML
serialization and parsing, over six tree shapes.

Google Benchmark is an optional dependency. Without it this target is skipped and the rest of the
test tree builds as usual. The target is not registered with CTest, so `ctest` is unaffected.

## Shapes

Breadth and depth are varied independently. Building a `TreeData` copies every finished node once
per level it sits below the root, so a deep shape and a wide shape of the same node count do not
cost the same. Every node carries four attributes, and the leaves carry a content string.

| Shape | Definition | Nodes |
|---|---|---|
| `Small` | root with 12 leaves | 13 |
| `DeepChain` | chain, one child per node | 250 |
| `FlatWide` | root with 10 000 leaves | 10 001 |
| `Balanced` | branching factor 6, depth 6 | 55 987 |
| `LargeConfig` | branching factor 8, depth 6 | 299 593 |
| `MixedContent` | `FlatWide` scaled down, with content on the root | 1 001 |

`DeepChain` is capped at 250 because libxml2 refuses to parse a document nested deeper than 256
levels unless `XML_PARSE_HUGE` is passed, which `sup::xml` does not do. Anything deeper builds and
serializes but cannot be read back.

## Mixed content

`MixedContent` is the only shape whose root holds a content string *and* child elements. Parsing it
is super-quadratic in the number of children, roughly 8x the time for 2x the children, and the root
content does not survive the round trip: `AddXMLContent` reads the indentation whitespace libxml2
keeps between the children and overwrites the real content with it. The shape is therefore small,
and registered on the parsing benchmarks only. Do not scale it up expecting `FlatWide` behaviour.

## Running

```bash
taskset -c 2 ./test_bin/benchmark-tests \
      --benchmark_repetitions=5 --benchmark_display_aggregates_only=true \
      --benchmark_counters_tabular=true \
      --benchmark_out=benchmark-output.json --benchmark_out_format=json \
      2>&1 | tee benchmark.log
```

Pin the run and compare medians: CPU scaling is on and an unpinned run drifts between cores. Do not
measure a `COA_COVERAGE=ON` build, it is `-O0`. `--benchmark_filter='.*LargeConfig'` selects the
largest shape only; each shape is built on first use, so a filtered run pays for only what it needs.
The parsing benchmarks write their XML into the working directory and remove it at exit.

`BM_MoveTree` reports no throughput counters on purpose, a move is independent of the shape and a
bytes/s figure for it would only invite comparison with the copy.

`benchmark.log` holds the committed run. The JSON is not committed; keep two of them locally and
`tools/compare.py benchmarks old.json new.json` from the Google Benchmark sources diffs them.
