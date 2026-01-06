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
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef SUP_XML_TREEDATA_PARSER_H_
#define SUP_XML_TREEDATA_PARSER_H_

#include <sup/xml/tree_data.h>

#include <memory>

namespace sup
{
namespace xml
{
std::unique_ptr<TreeData> TreeDataFromFile(const std::string& filename);

std::unique_ptr<TreeData> TreeDataFromString(const std::string& xml_str);

}  // namespace xml

}  // namespace sup

#endif  // SUP_XML_TREEDATA_PARSER_H_
