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

#include "tree_data_serialize.h"

#include <sup/xml/exceptions.h>
#include <sup/xml/tree_data.h>
#include <sup/xml/tree_data_serialize_utils.h>
#include <sup/xml/xml_utils.h>

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

namespace sup
{
namespace xml
{
void TreeDataToFile(const std::string& file_name, const TreeData& tree_data)
{
  // Create a new XmlWriter for uri, with no compression.
  XMLTextWriterHandle h_writer(xmlNewTextWriterFilename(file_name.c_str(), 0));
  std::string message = "sup::xml::TreeDataToFile(): could not create an XML writer for file [" +
    file_name;
  auto writer = AssertNoNullptr(h_writer.Writer(), SerializeException(message));
  SerializeUsingWriter(writer, tree_data);
}

std::string TreeDataToString(const TreeData& tree_data)
{
  // Create a new XML buffer, to which the XML document will be written
  XMLBufferHandle h_buffer{};
  std::string buf_message = "sup::xml::TreeDataToString(): could not create an XML buffer";
  auto buf = AssertNoNullptr(h_buffer.Buffer(), SerializeException(buf_message));

  // Create a new XmlWriter for memory, with no compression.
  XMLTextWriterHandle h_writer(xmlNewTextWriterMemory(buf, 0));
  std::string writer_message = "sup::xml::TreeDataToString(): could not create an XML writer";
  auto writer = AssertNoNullptr(h_writer.Writer(), SerializeException(writer_message));
  SerializeUsingWriter(writer, tree_data);
  return ToString(buf->content);
}

}  // namespace xml

}  // namespace sup
