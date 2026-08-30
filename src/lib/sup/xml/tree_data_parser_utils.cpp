/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : Supervision and Automation System Utilities
 *
 * Description   : SUP XML utilities
 *
 * Author        : Walter Van Herck (IO)
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

#include "tree_data_parser_utils.h"

#include <sup/xml/exceptions.h>
#include <sup/xml/xml_utils.h>

#include <libxml/parser.h>

#include <fstream>
#include <stack>

namespace
{
using namespace sup::xml;

struct StackNode
{
  TreeData tree;
  xmlNodePtr next_child;  // cursor into the sibling list, advanced by NextChild
};

}  // unnamed namespace

namespace sup
{
namespace xml
{
bool FileExists(const std::string& filename)
{
  std::ifstream file_stream(filename);
  return file_stream.is_open();
}

std::unique_ptr<TreeData> ParseXMLDoc(xmlDocPtr doc)
{
  // Check root element
  xmlNodePtr root_node = xmlDocGetRootElement(doc);
  if (root_node == nullptr)
  {
    xmlFreeDoc(doc);
    std::string message = "sup::xml::ParseXMLDoc(): could not retrieve root element";
    throw ParseException(message);
  }
  auto data_tree = ParseDataTree(doc, root_node);
  xmlFreeDoc(doc);
  return data_tree;
}

std::unique_ptr<TreeData> ParseDataTree(xmlDocPtr doc, xmlNodePtr node)
{
  std::stack<StackNode> stack;
  stack.push({CreateTreeData(doc, node), node->children});

  while (!stack.empty())  // process each node
  {
    auto& top_node = stack.top();
    auto next_child = NextChild(top_node.next_child);
    if (next_child != nullptr)
    {
      stack.push({CreateTreeData(doc, next_child), next_child->children});
    }
    else
    {
      // Moved out before the pop, so a finished subtree is never deep copied.
      auto current_tree = std::move(top_node.tree);
      stack.pop();
      if (stack.empty())
      {
        return std::make_unique<TreeData>(std::move(current_tree));
      }
      stack.top().tree.AddChild(std::move(current_tree));
    }
  }
  return nullptr;
}

TreeData CreateTreeData(xmlDocPtr doc, xmlNodePtr node)
{
  auto result = TreeData{ToString(node->name)};
  AddXMLAttributes(result, node);
  AddXMLContent(result, doc, node);
  return result;
}

// Advances the cursor past the returned node, so walking all children costs a single pass.
xmlNodePtr NextChild(xmlNodePtr& cursor)
{
  while (cursor != nullptr)
  {
    auto child = cursor;
    cursor = cursor->next;
    if (child->type == XML_ELEMENT_NODE)
    {
      return child;
    }
  }
  return nullptr;
}

void AddXMLAttributes(TreeData& tree, const xmlNodePtr node)
{
  auto attribute = node->properties;
  while (attribute != nullptr)
  {
    auto name = ToString(attribute->name);
    auto xml_val = xmlGetProp(node, attribute->name);
    auto value = ToString(xml_val);
    xmlFree(xml_val);
    tree.AddAttribute(name, value);
    attribute = attribute->next;
  }
}

void AddXMLContent(TreeData& tree, xmlDocPtr doc, const xmlNodePtr node)
{
  auto child_node = node->children;
  while (child_node != nullptr)
  {
    if (child_node->type == XML_TEXT_NODE)
    {
      auto xml_content = xmlNodeListGetString(doc, child_node, 1);
      auto content = ToString(xml_content);
      xmlFree(xml_content);
      tree.SetContent(content);
    }
    else
    {
      // All other node types are ignored in this process
    }
    child_node = child_node->next;
  }
}

}  // namespace xml

}  // namespace sup
