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

#include <sup/xml/tree_data.h>

#include <benchmark/benchmark.h>

#include <utility>

namespace sup
{
namespace xml
{
namespace test
{

namespace
{

// Building the shape node by node through AddChild, which copies each child it is given.
void BM_BuildTree(benchmark::State& state, Shape shape)
{
  for ([[maybe_unused]] auto loop_state : state)
  {
    auto tree = CreateTree(shape);
    benchmark::DoNotOptimize(tree);
  }
  SetShapeCounters(state, shape);
}

// Deep copy of a whole tree, the operation the parser performs on every node it pops.
void BM_CopyTree(benchmark::State& state, Shape shape)
{
  const auto& tree = GetTree(shape);
  for ([[maybe_unused]] auto loop_state : state)
  {
    TreeData copy{tree};
    benchmark::DoNotOptimize(copy);
  }
  SetShapeCounters(state, shape);
}

// Two moves per iteration: the source has to be moved back to stay usable for the next one. No
// size counters are reported, a move does not touch the nodes and does not scale with the shape.
void BM_MoveTree(benchmark::State& state, Shape shape)
{
  TreeData source{GetTree(shape)};
  for ([[maybe_unused]] auto loop_state : state)
  {
    TreeData moved{std::move(source)};
    benchmark::DoNotOptimize(moved);
    source = std::move(moved);
  }
}

// Comparison of two equal trees, which is the case that traverses both of them completely.
void BM_CompareTrees(benchmark::State& state, Shape shape)
{
  const auto& tree = GetTree(shape);
  const TreeData other{tree};
  for ([[maybe_unused]] auto loop_state : state)
  {
    benchmark::DoNotOptimize(tree == other);
  }
  SetShapeCounters(state, shape);
}

BENCHMARK_CAPTURE(BM_BuildTree, Small, Shape::kSmall)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_BuildTree, DeepChain, Shape::kDeepChain)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_BuildTree, FlatWide, Shape::kFlatWide)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_BuildTree, Balanced, Shape::kBalanced)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_BuildTree, LargeConfig, Shape::kLargeConfig)->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_CopyTree, Small, Shape::kSmall)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_CopyTree, DeepChain, Shape::kDeepChain)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_CopyTree, FlatWide, Shape::kFlatWide)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_CopyTree, Balanced, Shape::kBalanced)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_CopyTree, LargeConfig, Shape::kLargeConfig)->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_MoveTree, Small, Shape::kSmall)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_MoveTree, DeepChain, Shape::kDeepChain)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_MoveTree, FlatWide, Shape::kFlatWide)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_MoveTree, Balanced, Shape::kBalanced)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_MoveTree, LargeConfig, Shape::kLargeConfig)->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_CompareTrees, Small, Shape::kSmall)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_CompareTrees, DeepChain, Shape::kDeepChain)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_CompareTrees, FlatWide, Shape::kFlatWide)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_CompareTrees, Balanced, Shape::kBalanced)->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_CompareTrees, LargeConfig, Shape::kLargeConfig)->Unit(benchmark::kMillisecond);

}  // namespace

}  // namespace test

}  // namespace xml

}  // namespace sup
