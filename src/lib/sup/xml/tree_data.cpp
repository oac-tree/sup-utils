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

#include "tree_data.h"

#include <sup/xml/exceptions.h>

#include <algorithm>

namespace
{
bool EqualAttributes(const std::vector<sup::xml::TreeData::Attribute>& left,
                     const std::vector<sup::xml::TreeData::Attribute>& right);
}  // unnamed namespace

namespace sup
{
namespace xml
{

TreeData::TreeData(const std::string& node_name)
  : m_node_name{node_name}
  , m_content{}
  , m_attributes{}
  , m_children{}
{}

TreeData::~TreeData() = default;

TreeData::TreeData(const TreeData& other) = default;
TreeData::TreeData(TreeData&& other) noexcept = default;

TreeData& TreeData::operator=(const TreeData& other) & = default;
TreeData& TreeData::operator=(TreeData&& other) & noexcept = default;

std::string TreeData::GetNodeName() const
{
  return m_node_name;
}

size_t TreeData::GetNumberOfAttributes() const
{
  return m_attributes.size();
}

bool TreeData::HasAttribute(const std::string& name) const
{
  auto it = std::find_if(m_attributes.begin(), m_attributes.end(),
                         [name](const Attribute& attr)
                         {
                           return attr.first == name;
                         });
  return it != m_attributes.end();
}

std::string TreeData::GetAttribute(const std::string& name) const
{
  auto it = std::find_if(m_attributes.begin(), m_attributes.end(),
                         [name](const Attribute& attr)
                         {
                           return attr.first == name;
                         });
  if (it == m_attributes.end())
  {
    std::string message = "TreeData::GetAttribute(): attribute with name [" +
      name + "] does not exist";
    throw InvalidOperationException(message);
  }
  return it->second;
}

const std::vector<TreeData::Attribute>& TreeData::Attributes() const &
{
  return m_attributes;
}

void TreeData::AddAttribute(const std::string& name, const std::string& value)
{
  if (HasAttribute(name))
  {
    std::string message = "TreeData::AddAttribute(): attribute with name [" +
      name + "] already exists";
    throw InvalidOperationException(message);
  }
  (void)m_attributes.emplace_back(name, value);
}

size_t TreeData::GetNumberOfChildren() const
{
  return m_children.size();
}

void TreeData::AddChild(const TreeData& child)
{
  m_children.push_back(child);
}

const std::vector<TreeData>& TreeData::Children() const &
{
  return m_children;
}

void TreeData::SetContent(const std::string& content)
{
  m_content = content;
}

std::string TreeData::GetContent() const
{
  return m_content;
}

bool operator==(const TreeData& left, const TreeData& right)
{
  if (left.GetNodeName() != right.GetNodeName())
  {
    return false;
  }
  if (left.GetContent() != right.GetContent())
  {
    return false;
  }
  if (!EqualAttributes(left.Attributes(), right.Attributes()))
  {
    return false;
  }
  return left.Children() == right.Children();
}

bool operator!=(const TreeData& left, const TreeData& right)
{
  return !(left == right);
}

}  // namespace xml

}  // namespace sup

namespace
{
bool EqualAttributes(const std::vector<sup::xml::TreeData::Attribute>& left,
                     const std::vector<sup::xml::TreeData::Attribute>& right)
{
  if (left.size() != right.size())
  {
    return false;
  }
  return std::is_permutation(left.begin(), left.end(), right.begin());
}
}  // unnamed namespace
