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

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <map>
#include <string>

namespace
{
using sup::xml::TreeData;
using sup::xml::test::Shape;

const std::size_t kSmallLeaves = 12;
// libxml2 rejects documents nested deeper than 256 levels unless XML_PARSE_HUGE is passed, which
// sup::xml does not do, so the chain has to stay below that limit.
const std::size_t kChainLength = 250;
const std::size_t kWideLeaves = 10000;
const std::size_t kBalancedBranching = 6;
const std::size_t kBalancedDepth = 6;
const std::size_t kLargeConfigBranching = 8;
const std::size_t kLargeConfigDepth = 6;
// Kept far below kWideLeaves: parsing mixed content is super-quadratic, see the README.
const std::size_t kMixedContentLeaves = 1000;

// Attributes go on every node, content only on the leaves. An element holding both content and
// child elements is mixed content, which the configuration files this library reads do not use and
// which the parser does not handle well; kMixedContent is the shape that measures it on purpose.
void DecorateNode(TreeData& node, std::size_t index, bool with_content)
{
  const auto suffix = std::to_string(index);
  node.AddAttribute("name", "element_" + suffix);
  node.AddAttribute("type", "float64");
  node.AddAttribute("index", suffix);
  node.AddAttribute("unit", "ampere");
  if (with_content)
  {
    node.SetContent("value_" + suffix);
  }
}

TreeData CreateNode(const std::string& node_name, std::size_t index, bool with_content)
{
  TreeData node{node_name};
  DecorateNode(node, index, with_content);
  return node;
}

// Single chain, built from the leaf upwards: node i is the only child of node i-1.
TreeData CreateChain(std::size_t length)
{
  auto node = CreateNode("leaf", length - 1, true);
  for (std::size_t i = length - 1; i > 0; --i)
  {
    auto parent = CreateNode(i == 1 ? "root" : "level", i - 1, false);
    parent.AddChild(node);
    node = parent;
  }
  return node;
}

TreeData CreateFlat(std::size_t leaves, bool mixed_content)
{
  auto root = CreateNode("root", 0, mixed_content);
  for (std::size_t i = 0; i < leaves; ++i)
  {
    root.AddChild(CreateNode("leaf", i, true));
  }
  return root;
}

TreeData CreateBalanced(std::size_t branching, std::size_t depth, std::size_t& index)
{
  auto node = CreateNode(depth == 0 ? "leaf" : "node", index++, depth == 0);
  for (std::size_t i = 0; depth > 0 && i < branching; ++i)
  {
    node.AddChild(CreateBalanced(branching, depth - 1, index));
  }
  return node;
}

std::size_t CountNodes(const TreeData& tree)
{
  std::size_t count = 1;
  for (const auto& child : tree.Children())
  {
    count += CountNodes(child);
  }
  return count;
}

std::string ShapeName(Shape shape)
{
  switch (shape)
  {
  case Shape::kSmall:
    return "small";
  case Shape::kDeepChain:
    return "deep_chain";
  case Shape::kFlatWide:
    return "flat_wide";
  case Shape::kBalanced:
    return "balanced";
  case Shape::kMixedContent:
    return "mixed_content";
  default:
    break;
  }
  return "large_config";
}

// Removes the files it handed out, so that a benchmark run leaves the working directory as it was.
class XMLFileCache
{
public:
  XMLFileCache() = default;
  ~XMLFileCache();

  XMLFileCache(const XMLFileCache& other) = delete;
  XMLFileCache(XMLFileCache&& other) = delete;
  XMLFileCache& operator=(const XMLFileCache& other) = delete;
  XMLFileCache& operator=(XMLFileCache&& other) = delete;

  const std::string& GetFile(Shape shape);

private:
  std::map<Shape, std::string> m_file_names;
};

XMLFileCache::~XMLFileCache()
{
  for (const auto& entry : m_file_names)
  {
    (void)std::remove(entry.second.c_str());
  }
}

const std::string& XMLFileCache::GetFile(Shape shape)
{
  auto it = m_file_names.find(shape);
  if (it == m_file_names.end())
  {
    const auto file_name = "benchmark_" + ShapeName(shape) + ".xml";
    std::ofstream file_stream(file_name);
    file_stream << sup::xml::test::GetXML(shape);
    it = m_file_names.emplace(shape, file_name).first;
  }
  return it->second;
}

}  // unnamed namespace

namespace sup
{
namespace xml
{
namespace test
{

TreeData CreateTree(Shape shape)
{
  std::size_t index = 0;
  switch (shape)
  {
  case Shape::kSmall:
    return CreateFlat(kSmallLeaves, false);
  case Shape::kDeepChain:
    return CreateChain(kChainLength);
  case Shape::kFlatWide:
    return CreateFlat(kWideLeaves, false);
  case Shape::kBalanced:
    return CreateBalanced(kBalancedBranching, kBalancedDepth, index);
  case Shape::kMixedContent:
    return CreateFlat(kMixedContentLeaves, true);
  default:
    break;
  }
  return CreateBalanced(kLargeConfigBranching, kLargeConfigDepth, index);
}

const TreeData& GetTree(Shape shape)
{
  // std::map is used for the caches: it does not invalidate references when another shape is added.
  static std::map<Shape, TreeData> trees;
  auto it = trees.find(shape);
  if (it == trees.end())
  {
    it = trees.emplace(shape, CreateTree(shape)).first;
  }
  return it->second;
}

const std::string& GetXML(Shape shape)
{
  static std::map<Shape, std::string> xml_strings;
  auto it = xml_strings.find(shape);
  if (it == xml_strings.end())
  {
    it = xml_strings.emplace(shape, TreeDataToString(GetTree(shape))).first;
  }
  return it->second;
}

const std::string& GetXMLFile(Shape shape)
{
  static XMLFileCache file_cache;
  return file_cache.GetFile(shape);
}

std::size_t GetNodeCount(Shape shape)
{
  static std::map<Shape, std::size_t> node_counts;
  auto it = node_counts.find(shape);
  if (it == node_counts.end())
  {
    it = node_counts.emplace(shape, CountNodes(GetTree(shape))).first;
  }
  return it->second;
}

void SetShapeCounters(benchmark::State& state, Shape shape)
{
  state.SetBytesProcessed(static_cast<std::int64_t>(GetXML(shape).size()) * state.iterations());
  state.counters["nodes"] = benchmark::Counter(static_cast<double>(GetNodeCount(shape)),
                                               benchmark::Counter::kIsIterationInvariantRate);
}

}  // namespace test

}  // namespace xml

}  // namespace sup
