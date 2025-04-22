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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_XML_TREE_DATA_SERIALIZE_UTILS_H_
#define SUP_XML_TREE_DATA_SERIALIZE_UTILS_H_

#include <sup/xml/tree_data.h>

#include <libxml/xmlwriter.h>

namespace sup
{
namespace xml
{
class XMLBufferHandle
{
public:
  XMLBufferHandle();
  ~XMLBufferHandle();

  XMLBufferHandle(const XMLBufferHandle&) = delete;
  XMLBufferHandle& operator=(const XMLBufferHandle&) = delete;

  xmlBufferPtr Buffer() const &;
private:
  xmlBufferPtr m_buffer;
};

class XMLTextWriterHandle
{
public:
  explicit XMLTextWriterHandle(xmlTextWriterPtr writer);
  ~XMLTextWriterHandle();

  XMLTextWriterHandle(const XMLTextWriterHandle&) = delete;
  XMLTextWriterHandle& operator=(const XMLTextWriterHandle&) = delete;

  xmlTextWriterPtr Writer() const &;
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
