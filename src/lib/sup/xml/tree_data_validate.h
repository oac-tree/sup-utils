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

#ifndef SUP_XML_TREE_DATA_VALIDATE_H_
#define SUP_XML_TREE_DATA_VALIDATE_H_

#include <sup/xml/tree_data.h>

#include <string>

namespace sup
{
namespace xml
{

void ValidateSingleChildWithTag(const TreeData& tree, const std::string& child_tag);

void ValidateAllowedChildTags(const TreeData& tree, const std::vector<std::string> allowed_tags);

void ValidateNoAttributes(const TreeData& tree);

void ValidateNoChildren(const TreeData& tree);

void ValidateNoContent(const TreeData& tree);

}  // namespace xml

}  // namespace sup

#endif  // SUP_XML_TREE_DATA_VALIDATE_H_
