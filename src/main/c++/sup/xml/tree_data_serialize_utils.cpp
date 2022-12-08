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
#include <sup/xml/tree_data.h>
#include <sup/xml/xml_utils.h>

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

namespace
{
//! Serialize the TreeData to the given writer and release the writer
void SerializeAndFree(xmlTextWriterPtr writer, const sup::xml::TreeData& tree_data);

//! Set-up indentation.
void SetupWriterIndentation(xmlTextWriterPtr writer);

//! Main method for recursive writing of XML from TreeData.
void AddTreeData(xmlTextWriterPtr writer, const sup::xml::TreeData& tree_data);

//! Adds to currently opened XML element all attributes defined in TreeData.
void AddTreeAttributes(xmlTextWriterPtr writer, const sup::xml::TreeData& tree_data);

}  // namespace

namespace sup
{
namespace xml
{
void TreeDataToFile(const std::string& file_name, const TreeData& tree_data)
{
  xmlTextWriterPtr writer;

  // Create a new XmlWriter for uri, with no compression.
  writer = xmlNewTextWriterFilename(file_name.c_str(), 0);
  if (!writer)
  {
    std::string message = "sup::xml::TreeDataToFile(): could not create an XML writer for file [" +
    file_name;
    throw SerializeException(message);
  }
  SerializeAndFree(writer, tree_data);
}

std::string TreeDataToString(const TreeData& tree_data)
{
  xmlTextWriterPtr writer;
  xmlBufferPtr buf;

  // Create a new XML buffer, to which the XML document will be written
  buf = xmlBufferCreate();
  if (!buf)
  {
    std::string message = "sup::xml::TreeDataToString(): could not create an XML buffer";
    throw SerializeException(message);
  }

  // Create a new XmlWriter for memory, with no compression.
  writer = xmlNewTextWriterMemory(buf, 0);
  if (!writer)
  {
    std::string message = "sup::xml::TreeDataToString(): could not create an XML writer";
    throw SerializeException(message);
  }
  try
  {
    SerializeAndFree(writer, tree_data);
  }
  catch(const SerializeException&)
  {
    xmlBufferFree(buf);
    throw;
  }
  auto result = ToString(buf->content);
  xmlBufferFree(buf);
  return result;
}

}  // namespace xml

}  // namespace sup

namespace
{
void SerializeAndFree(xmlTextWriterPtr writer, const sup::xml::TreeData& tree_data)
{
  SetupWriterIndentation(writer);
  xmlTextWriterStartDocument(writer, nullptr, "UTF-8", nullptr);

  try
  {
    AddTreeData(writer, tree_data);
  }
  catch(const sup::xml::SerializeException&)
  {
    xmlTextWriterEndDocument(writer);
    xmlFreeTextWriter(writer);
    throw;
  }

  xmlTextWriterEndDocument(writer);
  xmlFreeTextWriter(writer);
}

void SetupWriterIndentation(xmlTextWriterPtr writer)
{
  const int indentation_on = 1;
  xmlTextWriterSetIndent(writer, indentation_on);
  xmlTextWriterSetIndentString(writer, sup::xml::FromString(std::string("  ")));
}

void AddTreeData(xmlTextWriterPtr writer, const sup::xml::TreeData& tree_data)
{
  using sup::xml::FromString;

  if (tree_data.GetNodeName().empty())
  {
    std::string message = "sup::xml::AddTreeData(): TreeData node has no name";
    throw sup::xml::SerializeException(message);
  }

  // opening element
  int rc = xmlTextWriterStartElement(writer, FromString(tree_data.GetNodeName()));
  if (rc < 0)
  {
    std::string message = "sup::xml::AddTreeData(): Error at xmlTextWriterStartElement";
    throw sup::xml::SerializeException(message);
  }

  // writing attribute
  AddTreeAttributes(writer, tree_data);

  // writing content
  if (!tree_data.GetContent().empty())
  {
    rc = xmlTextWriterWriteString(writer, FromString(tree_data.GetContent()));
    if (rc < 0)
    {
      std::string message = "sup::xml::AddTreeData(): Error at xmlTextWriterWriteString";
      throw sup::xml::SerializeException(message);
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
    std::string message = "sup::xml::AddTreeData(): Error at xmlTextWriterEndElement";
    throw sup::xml::SerializeException(message);
  }
}

void AddTreeAttributes(xmlTextWriterPtr writer, const sup::xml::TreeData& tree_data)
{
  for (const auto& attr : tree_data.Attributes())
  {
    int rc = xmlTextWriterWriteAttribute(writer, sup::xml::FromString(attr.first), sup::xml::FromString(attr.second));
    if (rc < 0)
    {
      std::string message = "sup::xml::AddTreeAttributes(): Error at xmlTextWriterWriteAttribute";
      throw sup::xml::SerializeException(message);
    }
  }
}

}  // namespace
