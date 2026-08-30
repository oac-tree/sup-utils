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

#include <sup/xml/tree_data_serialize.h>

#include <benchmark/benchmark.h>

#include <cstdio>
#include <string>

namespace sup
{
namespace xml
{
namespace test
{

namespace
{

const std::string kOutputFileName = "benchmark_serialize_output.xml";

// Serialization into an in-memory buffer.
void BM_TreeDataToString(benchmark::State& state, Shape shape)
{
  const auto& tree = GetTree(shape);
  for ([[maybe_unused]] auto loop_state : state)
  {
    auto xml = TreeDataToString(tree);
    benchmark::DoNotOptimize(xml);
  }
  SetShapeCounters(state, shape);
}

// Serialization onto the file system, which adds the write of the same bytes to the measurement.
void BM_TreeDataToFile(benchmark::State& state, Shape shape)
{
  const auto& tree = GetTree(shape);
  for ([[maybe_unused]] auto loop_state : state)
  {
    TreeDataToFile(kOutputFileName, tree);
  }
  (void)std::remove(kOutputFileName.c_str());
  SetShapeCounters(state, shape);
}

BENCHMARK_CAPTURE(BM_TreeDataToString, Small, Shape::kSmall)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_TreeDataToString, DeepChain, Shape::kDeepChain)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_TreeDataToString, FlatWide, Shape::kFlatWide)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_TreeDataToString, Balanced, Shape::kBalanced)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_TreeDataToString, LargeConfig, Shape::kLargeConfig)
    ->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_TreeDataToFile, Small, Shape::kSmall)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_TreeDataToFile, DeepChain, Shape::kDeepChain)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_TreeDataToFile, FlatWide, Shape::kFlatWide)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_TreeDataToFile, Balanced, Shape::kBalanced)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_TreeDataToFile, LargeConfig, Shape::kLargeConfig)
    ->Unit(benchmark::kMillisecond);

}  // namespace

}  // namespace test

}  // namespace xml

}  // namespace sup
