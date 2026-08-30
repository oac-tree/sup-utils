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

#ifndef SUP_XML_TREE_SHAPE_FIXTURES_H_
#define SUP_XML_TREE_SHAPE_FIXTURES_H_

#include <sup/xml/tree_data.h>

#include <benchmark/benchmark.h>

#include <cstddef>
#include <string>

namespace sup
{
namespace xml
{
namespace test
{

/**
 * @brief Tree shapes measured by the benchmarks.
 *
 * @details The shapes vary in two directions, how many children a node has and how deep the
 * nesting goes, because parsing cost does not follow node count alone.
 */
enum class Shape
{
  kSmall,         //!< root with 12 leaves
  kDeepChain,     //!< chain of 250 nodes, one child per node
  kFlatWide,      //!< root with 10000 leaves
  kBalanced,      //!< branching factor 6, depth 6, 55987 nodes
  kLargeConfig,   //!< branching factor 8, depth 6, 299593 nodes
  kMixedContent   //!< root with content and 1000 leaves, the only shape using mixed content
};

/**
 * @brief Builds the given shape.
 *
 * @param shape the shape to build
 * @return the newly built tree
 *
 * @details Every node carries four attributes, and the leaves carry a content string, so that the
 * serialized size scales with the node count instead of with the tag names alone.
 */
TreeData CreateTree(Shape shape);

/**
 * @brief Returns the tree of the given shape.
 *
 * @param shape the shape to build
 * @return reference to the cached tree
 *
 * @note Built on first use and cached for the lifetime of the process, so that building it is
 * never timed and never repeated per benchmark repetition.
 */
const TreeData& GetTree(Shape shape);

/**
 * @brief Returns the XML representation of GetTree(shape).
 *
 * @param shape the shape to serialize
 * @return reference to the cached XML string
 */
const std::string& GetXML(Shape shape);

/**
 * @brief Returns the name of a file holding GetXML(shape).
 *
 * @param shape the shape to serialize
 * @return reference to the cached file name
 *
 * @details The file is written into the working directory on first use and removed at exit.
 */
const std::string& GetXMLFile(Shape shape);

/**
 * @brief Returns the number of nodes of the given shape.
 *
 * @param shape the shape to count
 * @return number of nodes, the root included
 */
std::size_t GetNodeCount(Shape shape);

/**
 * @brief Reports the size of the shape on the benchmark state.
 *
 * @param state the state to report on
 * @param shape the measured shape
 *
 * @details Sets the processed bytes to the XML size and adds a nodes/s counter, so that shapes of
 * different sizes can be compared per node rather than per iteration.
 */
void SetShapeCounters(benchmark::State& state, Shape shape);

}  // namespace test

}  // namespace xml

}  // namespace sup

#endif  // SUP_XML_TREE_SHAPE_FIXTURES_H_
