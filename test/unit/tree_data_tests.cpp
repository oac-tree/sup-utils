/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : Supervision and Automation System Utilities
 *
 * Description   : SUP XML
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

#include "unit_test_helper.h"

#include <sup/xml/exceptions.h>
#include <sup/xml/tree_data.h>

#include <gtest/gtest.h>

using namespace sup::xml;

const std::string MAIN_NODE_NAME = "MainNode";
const std::string NODE_NAME_1 = "DATA_1";
const std::string NODE_NAME_2 = "DATA_2";
const std::string CHILD_NODE_NAME = "ChildNode";

const std::string NAME_ATTRIBUTE = "Name";
const std::string ID_ATTRIBUTE = "ID";
const std::string NAME_ATTRIBUTE_VALUE = "Some Name";
const std::string ID_ATTRIBUTE_VALUE = "1234";

class TreeDataTest : public ::testing::Test
{
protected:
  TreeDataTest();
  virtual ~TreeDataTest();
};

TEST_F(TreeDataTest, Construction)
{
  // Construct from name
  TreeData tree_data{MAIN_NODE_NAME};
  EXPECT_EQ(tree_data.GetNodeName(), MAIN_NODE_NAME);
  EXPECT_EQ(tree_data.GetNumberOfAttributes(), 0);
  EXPECT_EQ(tree_data.GetNumberOfChildren(), 0);
  auto attributes = tree_data.Attributes();
  EXPECT_EQ(attributes.size(), 0);
  auto children = tree_data.Children();
  EXPECT_EQ(children.size(), 0);
  EXPECT_TRUE(tree_data.GetContent().empty());

  // Copy construct
  TreeData tree_data_copy{tree_data};
  EXPECT_EQ(tree_data_copy.GetNodeName(), MAIN_NODE_NAME);
  EXPECT_EQ(tree_data_copy.GetNumberOfAttributes(), 0);
  EXPECT_EQ(tree_data_copy.GetNumberOfChildren(), 0);
  attributes = tree_data_copy.Attributes();
  EXPECT_EQ(attributes.size(), 0);
  children = tree_data_copy.Children();
  EXPECT_EQ(children.size(), 0);
  EXPECT_TRUE(tree_data_copy.GetContent().empty());

  // Move construct
  TreeData tree_data_move{std::move(tree_data_copy)};
  EXPECT_EQ(tree_data_move.GetNodeName(), MAIN_NODE_NAME);
  EXPECT_EQ(tree_data_move.GetNumberOfAttributes(), 0);
  EXPECT_EQ(tree_data_move.GetNumberOfChildren(), 0);
  attributes = tree_data_move.Attributes();
  EXPECT_EQ(attributes.size(), 0);
  children = tree_data_move.Children();
  EXPECT_EQ(children.size(), 0);
  EXPECT_TRUE(tree_data_move.GetContent().empty());
}

TEST_F(TreeDataTest, Equality)
{
  {
    // Compare nodes without children, attributes or content
    TreeData empty_1{NODE_NAME_1};
    TreeData empty_2{NODE_NAME_2};
    TreeData empty_3{NODE_NAME_1};
    EXPECT_NE(empty_1, empty_2);
    EXPECT_TRUE(empty_1 != empty_2);
    EXPECT_FALSE(empty_1 == empty_2);

    EXPECT_EQ(empty_1, empty_3);
    EXPECT_FALSE(empty_1 != empty_3);
    EXPECT_TRUE(empty_1 == empty_3);

    EXPECT_NE(empty_3, empty_2);
    EXPECT_TRUE(empty_3 != empty_2);
    EXPECT_FALSE(empty_3 == empty_2);
  }
  {
    // Compare nodes with/without content
    TreeData tree_1{NODE_NAME_1};
    TreeData tree_2{NODE_NAME_1};
    EXPECT_EQ(tree_1, tree_2);

    tree_1.SetContent("Content");
    EXPECT_NE(tree_1, tree_2);
    EXPECT_TRUE(tree_1 != tree_2);
    EXPECT_FALSE(tree_1 == tree_2);

    tree_2.SetContent("Wrong Content");
    EXPECT_NE(tree_1, tree_2);
    EXPECT_TRUE(tree_1 != tree_2);
    EXPECT_FALSE(tree_1 == tree_2);

    tree_2.SetContent("Content");
    EXPECT_EQ(tree_1, tree_2);
    EXPECT_FALSE(tree_1 != tree_2);
    EXPECT_TRUE(tree_1 == tree_2);
  }
  {
    // Compare nodes with/without attributes
    TreeData tree_1{NODE_NAME_1};
    TreeData tree_2{NODE_NAME_1};
    EXPECT_EQ(tree_1, tree_2);

    tree_1.AddAttribute(NAME_ATTRIBUTE, NAME_ATTRIBUTE_VALUE);
    EXPECT_NE(tree_1, tree_2);
    EXPECT_TRUE(tree_1 != tree_2);
    EXPECT_FALSE(tree_1 == tree_2);

    tree_2.AddAttribute(ID_ATTRIBUTE, ID_ATTRIBUTE_VALUE);
    EXPECT_NE(tree_1, tree_2);
    EXPECT_TRUE(tree_1 != tree_2);
    EXPECT_FALSE(tree_1 == tree_2);

    tree_2.AddAttribute(NAME_ATTRIBUTE, NAME_ATTRIBUTE_VALUE);
    EXPECT_NE(tree_1, tree_2);
    EXPECT_TRUE(tree_1 != tree_2);
    EXPECT_FALSE(tree_1 == tree_2);

    tree_1.AddAttribute(ID_ATTRIBUTE, ID_ATTRIBUTE_VALUE);
    EXPECT_EQ(tree_1, tree_2);
    EXPECT_FALSE(tree_1 != tree_2);
    EXPECT_TRUE(tree_1 == tree_2);
  }
  {
    // Compare nodes with/without children
    TreeData child_1{CHILD_NODE_NAME};
    TreeData child_2{CHILD_NODE_NAME};
    child_2.SetContent("child 2 content");

    TreeData tree_1{NODE_NAME_1};
    TreeData tree_2{NODE_NAME_1};
    EXPECT_EQ(tree_1, tree_2);

    tree_1.AddChild(child_1);
    EXPECT_NE(tree_1, tree_2);
    EXPECT_TRUE(tree_1 != tree_2);
    EXPECT_FALSE(tree_1 == tree_2);

    tree_2.AddChild(child_1);
    EXPECT_EQ(tree_1, tree_2);
    EXPECT_FALSE(tree_1 != tree_2);
    EXPECT_TRUE(tree_1 == tree_2);

    tree_2.AddChild(child_2);
    EXPECT_NE(tree_1, tree_2);
    EXPECT_TRUE(tree_1 != tree_2);
    EXPECT_FALSE(tree_1 == tree_2);

    tree_1.AddChild(child_2);
    EXPECT_EQ(tree_1, tree_2);
    EXPECT_FALSE(tree_1 != tree_2);
    EXPECT_TRUE(tree_1 == tree_2);
  }
}

TEST_F(TreeDataTest, Assignment)
{
  TreeData tree_1{NODE_NAME_1};
  TreeData child_1{CHILD_NODE_NAME};
  child_1.AddAttribute(NAME_ATTRIBUTE, NAME_ATTRIBUTE_VALUE);
  tree_1.AddChild(child_1);
  TreeData child_2{CHILD_NODE_NAME};
  child_2.SetContent("child 2 content");
  tree_1.AddChild(child_2);

  // Copy assignment
  TreeData tree_2{NODE_NAME_2};
  EXPECT_NE(tree_1, tree_2);
  tree_2 = tree_1;
  EXPECT_EQ(tree_1, tree_2);

  // Move assignment
  TreeData tree_3{NODE_NAME_2};
  EXPECT_NE(tree_1, tree_3);
  tree_3 = std::move(tree_2);
  EXPECT_EQ(tree_1, tree_3);
}

TEST_F(TreeDataTest, NodeName)
{
  TreeData tree_1{NODE_NAME_1};
  TreeData tree_2{NODE_NAME_2};
  EXPECT_EQ(tree_1.GetNodeName(), NODE_NAME_1);
  EXPECT_NE(tree_1.GetNodeName(), NODE_NAME_2);
  EXPECT_EQ(tree_2.GetNodeName(), NODE_NAME_2);
  EXPECT_NE(tree_2.GetNodeName(), NODE_NAME_1);
}

TEST_F(TreeDataTest, Attributes)
{
  // No attributes
  TreeData tree{NODE_NAME_1};
  EXPECT_EQ(tree.GetNumberOfAttributes(), 0);
  EXPECT_EQ(tree.Attributes().size(), 0);
  EXPECT_FALSE(tree.HasAttribute(NAME_ATTRIBUTE));
  EXPECT_FALSE(tree.HasAttribute(ID_ATTRIBUTE));
  EXPECT_THROW(tree.GetAttribute(NAME_ATTRIBUTE), InvalidOperationException);
  EXPECT_THROW(tree.GetAttribute(ID_ATTRIBUTE), InvalidOperationException);

  // Add name attribute
  EXPECT_NO_THROW(tree.AddAttribute(NAME_ATTRIBUTE, NAME_ATTRIBUTE_VALUE));
  EXPECT_EQ(tree.GetNumberOfAttributes(), 1);
  EXPECT_TRUE(tree.HasAttribute(NAME_ATTRIBUTE));
  EXPECT_FALSE(tree.HasAttribute(ID_ATTRIBUTE));
  auto name_val = tree.GetAttribute(NAME_ATTRIBUTE);
  EXPECT_EQ(name_val, NAME_ATTRIBUTE_VALUE);
  EXPECT_THROW(tree.GetAttribute(ID_ATTRIBUTE), InvalidOperationException);

  // Add id attribute
  EXPECT_NO_THROW(tree.AddAttribute(ID_ATTRIBUTE, ID_ATTRIBUTE_VALUE));
  EXPECT_EQ(tree.GetNumberOfAttributes(), 2);
  EXPECT_TRUE(tree.HasAttribute(NAME_ATTRIBUTE));
  EXPECT_TRUE(tree.HasAttribute(ID_ATTRIBUTE));
  name_val = tree.GetAttribute(NAME_ATTRIBUTE);
  EXPECT_EQ(name_val, NAME_ATTRIBUTE_VALUE);
  auto id_val = tree.GetAttribute(ID_ATTRIBUTE);
  EXPECT_EQ(id_val, ID_ATTRIBUTE_VALUE);

  // Try to add attribute with existing name
  EXPECT_THROW(tree.AddAttribute(NAME_ATTRIBUTE, "does not matter"), InvalidOperationException);
  EXPECT_THROW(tree.AddAttribute(ID_ATTRIBUTE, "does not matter"), InvalidOperationException);
  EXPECT_EQ(tree.GetNumberOfAttributes(), 2);
}

TEST_F(TreeDataTest, Children)
{
  // No attributes
  TreeData tree{NODE_NAME_1};
  EXPECT_EQ(tree.GetNumberOfChildren(), 0);
  EXPECT_EQ(tree.Children().size(), 0);

  // Add first child
  TreeData child_1{CHILD_NODE_NAME};
  child_1.AddAttribute(NAME_ATTRIBUTE, NAME_ATTRIBUTE_VALUE);
  tree.AddChild(child_1);
  EXPECT_EQ(tree.GetNumberOfChildren(), 1);
  auto children = tree.Children();
  ASSERT_EQ(children.size(), 1);
  EXPECT_EQ(children[0], child_1);

  // Add second child
  TreeData child_2{CHILD_NODE_NAME};
  child_2.SetContent("child 2 content");
  tree.AddChild(child_2);
  EXPECT_EQ(tree.GetNumberOfChildren(), 2);
  children = tree.Children();
  ASSERT_EQ(children.size(), 2);
  EXPECT_EQ(children[0], child_1);
  EXPECT_EQ(children[1], child_2);
}

TreeDataTest::TreeDataTest() = default;

TreeDataTest::~TreeDataTest() = default;
