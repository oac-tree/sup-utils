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

#ifndef SUP_XML_TREE_DATA_H_
#define SUP_XML_TREE_DATA_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

namespace sup
{
namespace xml
{
/**
 * @brief In-memory representation of an XML tree.
 */
class TreeData
{
public:
  using Attribute = std::pair<std::string, std::string>;

  /**
   * @brief Constructor.
   *
   * @param node_name Name of the current node.
   */
  TreeData(const std::string& node_name);

  ~TreeData();

  /**
   * @brief Copy/move Constructor.
   */
  TreeData(const TreeData& other);
  TreeData(TreeData&& other);

  /**
   * @brief Copy/move Assignment.
   */
  TreeData& operator=(const TreeData& other);
  TreeData& operator=(TreeData&& other);

  /**
   * @brief Comparison operators.
   */
  bool operator==(const TreeData& other) const;
  bool operator!=(const TreeData& other) const;

  /**
   * @brief Retrieve the name of the current node.
   *
   * @return Name of the current node.
   */
  std::string GetNodeName() const;

  /**
   * @brief Get number of attributes.
   *
   * @return Number of attributes.
   */
  size_t GetNumberOfAttributes() const;

  /**
   * @brief Indicate presence of attribute with given name.
   *
   * @param name Attribute name.
   *
   * @return true when present.
   */
  bool HasAttribute(const std::string& name) const;

  /**
   * @brief Get attribute with given name.
   *
   * @param name Attribute name.
   *
   * @return Attribute value when attribute exists.
   *
   * @throw InvalidOperationException when no attribute with the given name exists.
   */
  std::string GetAttribute(const std::string& name) const;

  /**
   * @brief Retrieve map of all attributes.
   *
   * @return List of all attributes.
   */
  const std::vector<Attribute>& Attributes() const;

  /**
   * @brief Add attribute with given name and value.
   *
   * @param name Attribute name.
   * @param value Attribute value.
   *
   * @return true on successful adding.
   *
   * @throw InvalidOperationException when an attribute with the given name already exists.
   */
  void AddAttribute(const std::string& name, const std::string& value);

  /**
   * @brief Get number of children.
   *
   * @return Number of children.
   */
  size_t GetNumberOfChildren() const;

  /**
   * @brief Add child element data.
   *
   * @param child Data representation of child element.
   */
  void AddChild(const TreeData& child);

  /**
   * @brief Retrieve all child data elements.
   *
   * @return List of child data elements.
   */
  const std::vector<TreeData>& Children() const;

  /**
   * @brief Set element content string.
   *
   * @param content Content string.
   *
   * @details Overwrites if already present.
   */
  void SetContent(const std::string& content);

  /**
   * @brief Retrieve element content string.
   *
   * @return Content string.
   */
  std::string GetContent() const;

private:
  std::string m_node_name;
  std::string m_content;
  std::vector<Attribute> m_attributes;
  std::vector<TreeData> m_children;
};

}  // namespace xml

}  // namespace sup

#endif  // SUP_XML_TREE_DATA_H_
