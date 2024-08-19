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

#ifndef SUP_XML_TREEDATA_PARSER_UTILS_H_
#define SUP_XML_TREEDATA_PARSER_UTILS_H_

#include <sup/xml/tree_data.h>

#include <libxml/parser.h>

#include <memory>

namespace sup
{
namespace xml
{

bool FileExists(const std::string& filename);

std::unique_ptr<TreeData> ParseXMLDoc(xmlDocPtr doc);

std::unique_ptr<TreeData> ParseDataTree(xmlDocPtr doc, const xmlNodePtr node);

void AddXMLAttributes(TreeData* tree, const xmlNodePtr node);

void AddXMLChildren(TreeData* tree, xmlDocPtr doc, const xmlNodePtr node);

}  // namespace xml

}  // namespace sup

#endif  // SUP_XML_TREEDATA_PARSER_UTILS_H_
