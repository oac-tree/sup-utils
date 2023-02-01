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

#include "tree_data_validate.h"

#include "exceptions.h"

#include <algorithm>

namespace sup
{
namespace xml
{

void ValidateSingleChildWithTag(const TreeData& tree, const std::string& child_tag)
{
  const auto& children = tree.Children();
  auto number_of_elements =
    std::count_if(children.begin(), children.end(),
                  [&child_tag](const TreeData& child)
                  {
                    return child.GetNodeName() == child_tag;
                  });
  if (number_of_elements != 1)
  {
    std::string error_message =
      "sup::xml::ValidateSingleChildWithTag(): element with tag [" + tree.GetNodeName() +
      "] requires exactly one child element with tag [" + child_tag + "]";
    throw ValidationException(error_message);
  }
}

void ValidateAllowedChildTags(const TreeData& tree,
                              const std::vector<std::string>& allowed_tags)
{
  for (const auto& child : tree.Children())
  {
    auto child_tag = child.GetNodeName();
    if (std::find(allowed_tags.begin(), allowed_tags.end(), child_tag) == allowed_tags.end())
    {
      std::string error_message =
        "sup::xml::ValidateAllowedChildTags(): element with tag [" + tree.GetNodeName() +
        "] must not contain child with tag [" + child_tag + "]";
      throw ValidationException(error_message);
    }
  }
}

void ValidateNoAttributes(const TreeData& tree)
{
  if (tree.Attributes().size() != 0)
  {
    std::string error_message =
      "sup::xml::ValidateNoAttributes(): element with tag [" + tree.GetNodeName() +
      "] must not have attributes";
    throw ValidationException(error_message);
  }
}

void ValidateNoChildren(const TreeData& tree)
{
  if (tree.Children().size() != 0)
  {
    std::string error_message =
      "sup::xml::ValidateNoChildren(): element with tag [" + tree.GetNodeName() +
      "] must not have children";
    throw ValidationException(error_message);
  }
}

void ValidateNoContent(const TreeData& tree)
{
  if (!tree.GetContent().empty())
  {
    std::string error_message =
      "sup::xml::ValidateNoContent(): element with tag [" + tree.GetNodeName() +
      "] must not have a content string";
    throw ValidationException(error_message);
  }
}

}  // namespace xml

}  // namespace sup
