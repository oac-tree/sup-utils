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

#include "unit_test_helper.h"

#include <sup/xml/tree_data.h>

#include <gtest/gtest.h>

using namespace sup::xml;

const std::string NODE_NAME_1 = "DATA_1";
const std::string NODE_NAME_2 = "DATA_2";
const std::string CHILD_NODE_NAME = "ChildNode";

class TreeDataTest : public ::testing::Test
{
protected:
  TreeDataTest();
  virtual ~TreeDataTest();

  TreeData m_data_1;
};

TEST_F(TreeDataTest, Construction)
{
  const std::string node_name = "MainNode";

  // Construct from name
  TreeData tree_data{node_name};
  EXPECT_EQ(tree_data.GetNodeName(), node_name);
  EXPECT_EQ(tree_data.GetNumberOfAttributes(), 0);
  EXPECT_EQ(tree_data.GetNumberOfChildren(), 0);
  auto attributes = tree_data.Attributes();
  EXPECT_EQ(attributes.size(), 0);
  auto children = tree_data.Children();
  EXPECT_EQ(children.size(), 0);
  EXPECT_TRUE(tree_data.GetContent().empty());

  // Copy construct
  TreeData tree_data_copy{tree_data};
  EXPECT_EQ(tree_data_copy.GetNodeName(), node_name);
  EXPECT_EQ(tree_data_copy.GetNumberOfAttributes(), 0);
  EXPECT_EQ(tree_data_copy.GetNumberOfChildren(), 0);
  attributes = tree_data_copy.Attributes();
  EXPECT_EQ(attributes.size(), 0);
  children = tree_data_copy.Children();
  EXPECT_EQ(children.size(), 0);
  EXPECT_TRUE(tree_data_copy.GetContent().empty());

  // Move construct
  TreeData tree_data_move{std::move(tree_data_copy)};
  EXPECT_EQ(tree_data_move.GetNodeName(), node_name);
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

    tree_1.AddAttribute("Name", "My name");
    EXPECT_NE(tree_1, tree_2);
    EXPECT_TRUE(tree_1 != tree_2);
    EXPECT_FALSE(tree_1 == tree_2);

    tree_2.AddAttribute("ID", "1234");
    EXPECT_NE(tree_1, tree_2);
    EXPECT_TRUE(tree_1 != tree_2);
    EXPECT_FALSE(tree_1 == tree_2);

    tree_2.AddAttribute("Name", "My name");
    EXPECT_NE(tree_1, tree_2);
    EXPECT_TRUE(tree_1 != tree_2);
    EXPECT_FALSE(tree_1 == tree_2);

    tree_1.AddAttribute("ID", "1234");
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

TreeDataTest::TreeDataTest()
  : m_data_1{NODE_NAME_1}
{
  const std::string child_node_name = "ChildNode";
  TreeData child_1{child_node_name};
  child_1.SetContent("child1");
  m_data_1.AddChild(child_1);
  TreeData child_2{child_node_name};
  child_2.AddAttribute("name", "child2");
  m_data_1.AddChild(child_2);
  m_data_1.AddAttribute("id", "1234");
  m_data_1.AddAttribute("name", "root");
  m_data_1.SetContent("Main content");
}

TreeDataTest::~TreeDataTest() = default;
