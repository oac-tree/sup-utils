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

#include "tree_data_parser.h"

#include <sup/xml/exceptions.h>
#include <sup/xml/tree_data_parser_utils.h>
#include <sup/xml/xml_utils.h>

#include <string.h>

namespace sup
{
namespace xml
{

std::unique_ptr<TreeData> TreeDataFromFile(const std::string& filename)
{
  // Read file into xmlDocPtr
  if (!FileExists(filename))
  {
    std::string message = "sup::xml::TreeDataFromFile(): file not found [" + filename + "]";
    throw ParseException(message);
  }
  xmlDocPtr doc = xmlReadFile(filename.c_str(), nullptr, XML_PARSE_NOBLANKS);
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
  xmlDocPtr doc = xmlReadDoc(FromString(xml_str), nullptr, nullptr, XML_PARSE_NOBLANKS);
  if (doc == nullptr)
  {
    auto xml_head = xml_str.substr(0, 1024);
    std::string message = "sup::xml::TreeDataFromString(): used xml library could not parse string "
    "[" + xml_head + "]";
    throw ParseException(message);
  }
  return ParseXMLDoc(doc);
}

}  // namespace xml

}  // namespace sup
