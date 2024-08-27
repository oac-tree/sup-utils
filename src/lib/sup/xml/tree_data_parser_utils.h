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

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <sup/xml/tree_data.h>

#include <memory>
#include <stack>
#include <string>
#include <utility>

namespace sup
{
namespace xml
{

struct stackData
{
  TreeData* current_tree;
  TreeData* parent_tree;
  xmlNodePtr current_node;
};

using childData = std::pair<TreeData, xmlNodePtr>;
using childDataPtr = std::pair<TreeData*, xmlNodePtr>;

bool FileExists(const std::string& filename);

std::unique_ptr<TreeData> ParseXMLDoc(xmlDocPtr doc);

std::unique_ptr<TreeData> ParseDataTree(xmlDocPtr doc, const xmlNodePtr node);

void addChildrenToStack(std::stack<stackData>& myStack, std::deque<childData>& childVector,
                        childDataPtr node);

void buildStack(std::stack<stackData>& myStack, std::deque<childData>& childVector,
                TreeData* startTree, xmlNodePtr startNode);

void AddXMLAttributes(TreeData* tree, const xmlNodePtr node);

void AddXMLContent(TreeData* tree, xmlDocPtr doc, const xmlNodePtr node);

}  // namespace xml

}  // namespace sup

#endif  // SUP_XML_TREEDATA_PARSER_UTILS_H_
