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

#ifndef SUP_XML_TREEDATA_PARSER_UTILS_H_
#define SUP_XML_TREEDATA_PARSER_UTILS_H_

#include <sup/xml/base_types.h>
#include <sup/xml/tree_data.h>

#include <libxml/tree.h>

#include <memory>
#include <string>

namespace sup
{
namespace xml
{

bool FileExists(const std::string& filename);

std::unique_ptr<TreeData> ParseXMLDoc(xmlDocPtr doc);

std::unique_ptr<TreeData> ParseDataTree(xmlDocPtr doc, const xmlNodePtr node);

TreeData CreateTreeData(xmlDocPtr doc, xmlNodePtr node);

xmlNodePtr NextChild(xmlNodePtr parent, uint32& next_child_idx);

void AddXMLAttributes(TreeData& tree, const xmlNodePtr node);

void AddXMLContent(TreeData& tree, xmlDocPtr doc, const xmlNodePtr node);

}  // namespace xml

}  // namespace sup

#endif  // SUP_XML_TREEDATA_PARSER_UTILS_H_
