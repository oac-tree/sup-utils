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
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "tree_data_parser_utils.h"
#include "base_types.h"

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
  xmlNodePtr xml_node;
  uint32 next_child_index;
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
  stack.push({CreateTreeData(doc, node), node, 0});

  while (!stack.empty())  // process each node
  {
    auto& top_node = stack.top();
    auto next_child = NextChild(top_node.xml_node, top_node.next_child_index);
    if (next_child != nullptr)
    {
      stack.push({CreateTreeData(doc, next_child), next_child, 0});
    }
    else
    {
      auto current_tree = top_node.tree;  // Creates a copy
      stack.pop();
      if (!stack.empty())
      {
        auto& parent = stack.top();
        parent.tree.AddChild(current_tree);
      }
      else
      {
        return std::make_unique<TreeData>(current_tree);
      }
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

xmlNodePtr NextChild(xmlNodePtr parent, uint32& next_child_idx)
{
  auto child = parent->children;
  uint32 child_idx = 0;
  while (child != nullptr)
  {
    if ((child_idx >= next_child_idx) && (child->type == XML_ELEMENT_NODE))
    {
      next_child_idx = ++child_idx;
      return child;
    }
    child = child->next;
    ++child_idx;
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
