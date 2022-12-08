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

#ifndef SUP_XML_XML_UTILS_H_
#define SUP_XML_XML_UTILS_H_

#include <libxml/xmlstring.h>

#include <string>

namespace sup
{
namespace xml
{
//! Converts xmlChar array to std::string.
std::string ToString(const xmlChar* xml_name);

//! Converts std::string to xmlChar array. The result array is still owned by the input string.
const xmlChar* FromString(const std::string& str);

}  // namespace xml

}  // namespace sup

#endif  // SUP_XML_XML_UTILS_H_
