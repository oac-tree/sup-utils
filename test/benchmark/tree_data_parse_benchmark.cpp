/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : Supervision and Automation System Utilities
 *
 * Description   : Performance test code
 *
 * Author        : Marcelo Mariano (External)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "tree_shape_fixtures.h"

#include <sup/xml/tree_data_parser.h>

#include <benchmark/benchmark.h>

namespace sup
{
namespace xml
{
namespace test
{

namespace
{

// Parsing from memory: libxml2 parsing plus the TreeData build-up, which is the operation the
// deep shapes are meant to stress.
void BM_TreeDataFromString(benchmark::State& state, Shape shape)
{
  const auto& xml = GetXML(shape);
  for ([[maybe_unused]] auto loop_state : state)
  {
    benchmark::DoNotOptimize(TreeDataFromString(xml));
  }
  SetShapeCounters(state, shape);
}

// Parsing from the file system, which adds the read of the same bytes to the measurement.
void BM_TreeDataFromFile(benchmark::State& state, Shape shape)
{
  const auto& file_name = GetXMLFile(shape);
  for ([[maybe_unused]] auto loop_state : state)
  {
    benchmark::DoNotOptimize(TreeDataFromFile(file_name));
  }
  SetShapeCounters(state, shape);
}

BENCHMARK_CAPTURE(BM_TreeDataFromString, Small, Shape::kSmall)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_TreeDataFromString, DeepChain, Shape::kDeepChain)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_TreeDataFromString, FlatWide, Shape::kFlatWide)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_TreeDataFromString, Balanced, Shape::kBalanced)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_TreeDataFromString, LargeConfig, Shape::kLargeConfig)
    ->Unit(benchmark::kMillisecond);
// Registered on the parsing benchmarks only: mixed content costs nothing to build or to serialize.
BENCHMARK_CAPTURE(BM_TreeDataFromString, MixedContent, Shape::kMixedContent)
    ->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_TreeDataFromFile, Small, Shape::kSmall)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_TreeDataFromFile, DeepChain, Shape::kDeepChain)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_TreeDataFromFile, FlatWide, Shape::kFlatWide)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_TreeDataFromFile, Balanced, Shape::kBalanced)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_TreeDataFromFile, LargeConfig, Shape::kLargeConfig)
    ->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_TreeDataFromFile, MixedContent, Shape::kMixedContent)
    ->Unit(benchmark::kMillisecond);

}  // namespace

}  // namespace test

}  // namespace xml

}  // namespace sup
