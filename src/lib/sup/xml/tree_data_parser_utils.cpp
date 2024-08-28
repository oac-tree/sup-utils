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
#include "sup/codec/base_types.h"

#include <sup/xml/exceptions.h>
#include <sup/xml/xml_utils.h>

#include <libxml/parser.h>

#include <fstream>
#include <stack>

namespace
{
using namespace sup::xml;

struct stackData
{
  TreeData tree;
  xmlNodePtr xml_node;
  sup::codec::uint32 next_child_index;
};

std::unique_ptr<TreeData> ParseDataTree(xmlDocPtr doc, const xmlNodePtr node);

bool addChildrenToStack(std::stack<stackData>& myStack);

void AddXMLAttributes(TreeData* tree, const xmlNodePtr node);

void AddXMLContent(TreeData* tree, xmlDocPtr doc, const xmlNodePtr node);

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

}  // namespace xml

}  // namespace sup

namespace
{

bool addChildrenToStack(std::stack<stackData>& myStack)
{
  auto topStack = myStack.top();
  auto child_node = topStack.xml_node->children;
  auto next_child_index = &myStack.top().next_child_index;
  sup::codec::uint32 child_index = 0;
  while (child_node != nullptr)
  {
    if ((child_index >= *next_child_index) && (child_node->type == XML_ELEMENT_NODE))
    {
      *next_child_index = ++child_index;
      myStack.push({TreeData(ToString(child_node->name)), child_node, 0});
      return true;
    }
    child_node = child_node->next;
    ++child_index;
  }
  return false;
}

std::unique_ptr<TreeData> ParseDataTree(xmlDocPtr doc, xmlNodePtr node)
{
  std::stack<stackData> myStack;

  myStack.push({TreeData(ToString(node->name)), node, 0});

  while (!myStack.empty())  // process each node
  {
    if (!addChildrenToStack(myStack))
    {
      auto stack_top = myStack.top();
      myStack.pop();

      auto current_tree = &stack_top.tree;
      auto current_node = stack_top.xml_node;

      AddXMLAttributes(current_tree, current_node);
      AddXMLContent(current_tree, doc, current_node);

      if (!myStack.empty())
      {
        auto parent_tree = &myStack.top().tree;
        parent_tree->AddChild(*current_tree);
      }
      else
      {
        std::unique_ptr<TreeData> result(new TreeData(*current_tree));
        return result;
      }
    }
  }
  return nullptr;
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

}  // unnamed namespace
