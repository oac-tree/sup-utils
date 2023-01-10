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

#include "tree_data_serialize_utils.h"

#include <sup/xml/exceptions.h>
#include <sup/xml/xml_utils.h>

namespace sup
{
namespace xml
{
void SerializeUsingWriter(xmlTextWriterPtr writer, const TreeData& tree_data)
{
  SetupWriterIndentation(writer);
  xmlTextWriterStartDocument(writer, nullptr, "UTF-8", nullptr);

  AddTreeData(writer, tree_data);

  xmlTextWriterEndDocument(writer);
}

void SetupWriterIndentation(xmlTextWriterPtr writer)
{
  const int indentation_on = 1;
  xmlTextWriterSetIndent(writer, indentation_on);
  xmlTextWriterSetIndentString(writer, FromString(std::string("  ")));
}

void AddTreeData(xmlTextWriterPtr writer, const TreeData& tree_data)
{
  if (tree_data.GetNodeName().empty())
  {
    std::string message = "AddTreeData(): TreeData node has no name";
    throw SerializeException(message);
  }

  // opening element
  int rc = xmlTextWriterStartElement(writer, FromString(tree_data.GetNodeName()));
  if (rc < 0)
  {
    std::string message = "AddTreeData(): Error at xmlTextWriterStartElement";
    throw SerializeException(message);
  }

  // writing attribute
  AddTreeAttributes(writer, tree_data);

  // writing content
  if (!tree_data.GetContent().empty())
  {
    rc = xmlTextWriterWriteString(writer, FromString(tree_data.GetContent()));
    if (rc < 0)
    {
      std::string message = "AddTreeData(): Error at xmlTextWriterWriteString";
      throw SerializeException(message);
    }
  }

  // writing children
  for (const auto& child : tree_data.Children())
  {
    AddTreeData(writer, child);
  }

  // closing element
  rc = xmlTextWriterEndElement(writer);
  if (rc < 0)
  {
    std::string message = "AddTreeData(): Error at xmlTextWriterEndElement";
    throw SerializeException(message);
  }
}

void AddTreeAttributes(xmlTextWriterPtr writer, const TreeData& tree_data)
{
  for (const auto& attr : tree_data.Attributes())
  {
    int rc = xmlTextWriterWriteAttribute(writer, FromString(attr.first), FromString(attr.second));
    if (rc < 0)
    {
      std::string message = "AddTreeAttributes(): Error at xmlTextWriterWriteAttribute";
      throw SerializeException(message);
    }
  }
}

}  // namespace xml

}  // namespace sup

