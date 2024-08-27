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
 * Copyright (c) : 2010-2023 ITER Organization,
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

#include <sup/xml/exceptions.h>
#include <sup/xml/xml_utils.h>

#include <fstream>

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

void addChildrenToStack(std::stack<stackData>& myStack, std::deque<childData>& childVector,
                        childDataPtr node)
{
  auto local_parent = node.second;
  auto child_node = local_parent->last;
  while (child_node != nullptr)
  {
    if (child_node->type == XML_ELEMENT_NODE)
    {
      childVector.push_back({TreeData(ToString(child_node->name)), child_node});
      myStack.push({&childVector.back().first, node.first, child_node});
    }
    child_node = child_node->prev;
  }
}

void buildStack(std::stack<stackData>& myStack, std::deque<childData>& childVector,
                TreeData* startTree, xmlNodePtr startNode)
{
  addChildrenToStack(myStack, childVector, {startTree, startNode});

  for (auto& child : childVector)
  {
    addChildrenToStack(myStack, childVector, {&child.first, child.second});
  }
}

std::unique_ptr<TreeData> ParseDataTree(xmlDocPtr doc, xmlNodePtr node)
{
  std::stack<stackData> myStack;
  std::deque<childData> childVector;
  std::unique_ptr<TreeData> startTree(new TreeData(ToString(node->name)));
  myStack.push({startTree.get(), nullptr, node});
  buildStack(myStack, childVector, startTree.get(), node);

  while (!myStack.empty())  // process each node
  {
    auto stack_top = myStack.top();
    myStack.pop();

    auto current_tree = stack_top.current_tree;
    auto parent_tree = stack_top.parent_tree;
    auto current_node = stack_top.current_node;

    AddXMLAttributes(current_tree, current_node);
    AddXMLContent(current_tree, doc, current_node);

    if (parent_tree)
    {
      parent_tree->AddChild(*current_tree);
    }
  }
  return startTree;
}

void AddXMLAttributes(TreeData* tree, const xmlNodePtr node)
{
  auto attribute = node->properties;
  while (attribute != nullptr)
  {
    auto name = ToString(attribute->name);
    auto xml_val = xmlGetProp(node, attribute->name);
    auto value = ToString(xml_val);
    xmlFree(xml_val);
    tree->AddAttribute(name, value);
    attribute = attribute->next;
  }
}

void AddXMLContent(TreeData* tree, xmlDocPtr doc, const xmlNodePtr node)
{
  auto child_node = node->children;
  while (child_node != nullptr)
  {
    if (child_node->type == XML_TEXT_NODE)
    {
      auto xml_content = xmlNodeListGetString(doc, child_node, 1);
      auto content = ToString(xml_content);
      xmlFree(xml_content);
      tree->SetContent(content);
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
