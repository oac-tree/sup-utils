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

#ifndef SUP_XML_TREE_DATA_SERIALIZE_UTILS_H_
#define SUP_XML_TREE_DATA_SERIALIZE_UTILS_H_

#include <sup/xml/tree_data.h>

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include <string>

namespace sup
{
namespace xml
{
class XMLBufferHandle
{
public:
  XMLBufferHandle() : m_buffer{xmlBufferCreate()} {}
  ~XMLBufferHandle() { if (m_buffer) xmlBufferFree(m_buffer); }

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
  ~XMLTextWriterHandle() { if (m_writer) xmlFreeTextWriter(m_writer); }

  XMLTextWriterHandle(const XMLTextWriterHandle&) = delete;
  XMLTextWriterHandle& operator=(const XMLTextWriterHandle&) = delete;

  xmlTextWriterPtr Writer() const { return m_writer; }
private:
  xmlTextWriterPtr m_writer;
};

//! Serialize the TreeData to the given writer
void SerializeUsingWriter(xmlTextWriterPtr writer, const sup::xml::TreeData& tree_data);

//! Set-up indentation.
void SetupWriterIndentation(xmlTextWriterPtr writer);

//! Main method for recursive writing of XML from TreeData.
void AddTreeData(xmlTextWriterPtr writer, const sup::xml::TreeData& tree_data);

//! Adds to currently opened XML element all attributes defined in TreeData.
void AddTreeAttributes(xmlTextWriterPtr writer, const sup::xml::TreeData& tree_data);

}  // namespace xml

}  // namespace sup

#endif  // SUP_XML_TREE_DATA_SERIALIZE_UTILS_H_
