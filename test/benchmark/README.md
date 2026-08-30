# Performance benchmarks

Google Benchmark measurements of `TreeData` build-up, copy, move and comparison, and of XML
serialization and parsing. Google Benchmark is optional: without it the target is skipped. It is
never registered with CTest, so `ctest` does not run it.

## Shapes

The shapes vary in two directions: how many children a node has, and how deep the nesting goes.
Parsing cost does not follow node count alone, so the two are varied separately. Every node carries
four attributes, and leaves also carry content.

| Shape | Definition | Nodes |
|---|---|---|
| `Small` | root with 12 leaves | 13 |
| `DeepChain` | chain, one child per node | 250 |
| `FlatWide` | root with 10 000 leaves | 10 001 |
| `Balanced` | branching factor 6, depth 6 | 55 987 |
| `LargeConfig` | branching factor 8, depth 6 | 299 593 |
| `MixedContent` | root with content *and* 1 000 leaves | 1 001 |

`DeepChain` cannot grow past 256. libxml2 refuses to parse a document nested deeper than that
without `XML_PARSE_HUGE`, which `sup::xml` does not pass, so deeper trees build and serialize but
cannot be read back.

`MixedContent` does not scale like `FlatWide`. An element holding both content and child elements
parses in super-quadratic time, and its content does not survive the round trip. It is small on
purpose and registered on the parsing benchmarks only.

## Running

```bash
taskset -c 2 ./test_bin/benchmark-tests \
      --benchmark_repetitions=5 --benchmark_display_aggregates_only=true \
      --benchmark_counters_tabular=true \
      --benchmark_out=benchmark-output.json --benchmark_out_format=json \
      2>&1 | tee benchmark.log
```

Pin the run and compare medians, CPU scaling is on. Never measure a `COA_COVERAGE=ON` build, it is
`-O0`. Shapes are built on first use, so `--benchmark_filter` pays only for what it selects. The
parsing benchmarks write their XML into the working directory and remove it at exit.

`benchmark.log` is the committed run. The JSON is not committed; `tools/compare.py` from the Google
Benchmark sources diffs two of them.
