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
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "tree_data_parser.h"

#include <sup/xml/exceptions.h>
#include <sup/xml/xml_utils.h>

#include <libxml/parser.h>

#include <fstream>
#include <stdio.h>
#include <string.h>

namespace sup
{
namespace xml
{
static bool FileExists(const std::string& filename);

static std::unique_ptr<TreeData> ParseXMLDoc(xmlDocPtr doc);

static std::unique_ptr<TreeData> ParseDataTree(xmlDocPtr doc, xmlNodePtr node);

static void AddXMLAttributes(TreeData* tree, xmlNodePtr node);

static void AddXMLChildren(TreeData* tree, xmlDocPtr doc, xmlNodePtr node);

std::unique_ptr<TreeData> TreeDataFromFile(const std::string& filename)
{
  // Read file into xmlDocPtr
  if (!FileExists(filename))
  {
    std::string message = "sup::xml::TreeDataFromFile(): file not found [" + filename + "]";
    throw ParseException(message);
  }
  xmlDocPtr doc = xmlParseFile(filename.c_str());
  if (doc == nullptr)
  {
    std::string message = "sup::xml::TreeDataFromFile(): used xml library could not parse file [" +
    filename + "]";
    throw ParseException(message);
  }
  return ParseXMLDoc(doc);
}

std::unique_ptr<TreeData> TreeDataFromString(const std::string& xml_str)
{
  // Read the string into xmlDocPtr
  xmlDocPtr doc = xmlParseDoc(FromString(xml_str));
  if (doc == nullptr)
  {
    auto xml_head = xml_str.substr(0, 1024);
    std::string message = "sup::xml::TreeDataFromString(): used xml library could not parse string "
    "[" + xml_head + "]";
    throw ParseException(message);
  }
  return ParseXMLDoc(doc);
}

static bool FileExists(const std::string& filename)
{
  std::ifstream file_stream(filename);
  return file_stream.is_open();
}

static std::unique_ptr<TreeData> ParseXMLDoc(xmlDocPtr doc)
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

static std::unique_ptr<TreeData> ParseDataTree(xmlDocPtr doc, xmlNodePtr node)
{
  auto node_name = ToString(node->name);
  std::unique_ptr<TreeData> result(new TreeData(node_name));

  AddXMLAttributes(result.get(), node);
  AddXMLChildren(result.get(), doc, node);

  return result;
}

static void AddXMLAttributes(TreeData *tree, xmlNodePtr node)
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

static void AddXMLChildren(TreeData *tree, xmlDocPtr doc, xmlNodePtr node)
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
    else if (child_node->type == XML_ELEMENT_NODE)
    {
      auto child_data = ParseDataTree(doc, child_node);
      tree->AddChild(*child_data);
    }
    child_node = child_node->next;
  }
}

}  // namespace xml

}  // namespace sup
