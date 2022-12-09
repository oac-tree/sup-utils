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
//! Serialize the TreeData to the given writer
void SerializeUsingWriter(xmlTextWriterPtr writer, const sup::xml::TreeData& tree_data);

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
class XMLBufferHandle
{
public:
  XMLBufferHandle() : m_buffer{xmlBufferCreate()} {}
  ~XMLBufferHandle() { xmlBufferFree(m_buffer); }

  XMLBufferHandle(const XMLBufferHandle&) = delete;
  XMLBufferHandle& operator=(const XMLBufferHandle&) = delete;

  xmlBufferPtr Buffer() const { return m_buffer; }
private:
  xmlBufferPtr m_buffer;
};

class XMLTextWriterHandle
{
public:
  XMLTextWriterHandle(xmlTextWriterPtr writer) : m_writer{writer} {}
  ~XMLTextWriterHandle() { xmlFreeTextWriter(m_writer); }

  XMLTextWriterHandle(const XMLTextWriterHandle&) = delete;
  XMLTextWriterHandle& operator=(const XMLTextWriterHandle&) = delete;

  xmlTextWriterPtr Writer() const { return m_writer; }
private:
  xmlTextWriterPtr m_writer;
};

void TreeDataToFile(const std::string& file_name, const TreeData& tree_data)
{
  // Create a new XmlWriter for uri, with no compression.
  XMLTextWriterHandle h_writer(xmlNewTextWriterFilename(file_name.c_str(), 0));
  auto writer = h_writer.Writer();
  if (!writer)
  {
    std::string message = "sup::xml::TreeDataToFile(): could not create an XML writer for file [" +
    file_name;
    throw SerializeException(message);
  }
  SerializeUsingWriter(writer, tree_data);
}

std::string TreeDataToString(const TreeData& tree_data)
{
  // Create a new XML buffer, to which the XML document will be written
  XMLBufferHandle h_buffer{};
  auto buf = h_buffer.Buffer();
  if (!buf)
  {
    std::string message = "sup::xml::TreeDataToString(): could not create an XML buffer";
    throw SerializeException(message);
  }

  // Create a new XmlWriter for memory, with no compression.
  XMLTextWriterHandle h_writer(xmlNewTextWriterMemory(buf, 0));
  auto writer = h_writer.Writer();
  if (!writer)
  {
    std::string message = "sup::xml::TreeDataToString(): could not create an XML writer";
    throw SerializeException(message);
  }
  SerializeUsingWriter(writer, tree_data);
  return ToString(buf->content);
}

}  // namespace xml

}  // namespace sup

namespace
{
void SerializeUsingWriter(xmlTextWriterPtr writer, const sup::xml::TreeData& tree_data)
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
