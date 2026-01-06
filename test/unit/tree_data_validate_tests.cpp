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

#include <sup/xml/exceptions.h>
#include <sup/xml/tree_data_validate.h>

#include <gtest/gtest.h>

using namespace sup::xml;

const std::string ROOT_ELEMENT_NAME = "root";
const std::string CHILD_ELEMENT_NAME = "child";

class TreeDataValidateTest : public ::testing::Test
{
protected:
  TreeDataValidateTest();
  virtual ~TreeDataValidateTest();
};

TEST_F(TreeDataValidateTest, SingleChildWithTag)
{
  TreeData tree{ROOT_ELEMENT_NAME};
  EXPECT_THROW(ValidateSingleChildWithTag(tree, CHILD_ELEMENT_NAME), ValidationException);
  TreeData child{CHILD_ELEMENT_NAME};
  tree.AddChild(child);
  EXPECT_NO_THROW(ValidateSingleChildWithTag(tree, CHILD_ELEMENT_NAME));
}

TEST_F(TreeDataValidateTest, AllowedChildTags)
{
  std::vector<std::string> allowed_tags = { CHILD_ELEMENT_NAME };
  TreeData tree{ROOT_ELEMENT_NAME};
  EXPECT_NO_THROW(ValidateAllowedChildTags(tree, allowed_tags));
  TreeData child{CHILD_ELEMENT_NAME};
  tree.AddChild(child);
  EXPECT_NO_THROW(ValidateAllowedChildTags(tree, allowed_tags));
  TreeData wrong_child{"NotAllowed"};
  tree.AddChild(wrong_child);
  EXPECT_THROW(ValidateAllowedChildTags(tree, allowed_tags), ValidationException);
}

TEST_F(TreeDataValidateTest, NoAttributes)
{
  TreeData tree{ROOT_ELEMENT_NAME};
  EXPECT_NO_THROW(ValidateNoAttributes(tree));
  tree.AddAttribute("NotAllowed", "DoesntMatter");
  EXPECT_THROW(ValidateNoAttributes(tree), ValidationException);
}

TEST_F(TreeDataValidateTest, NoChildren)
{
  TreeData tree{ROOT_ELEMENT_NAME};
  EXPECT_NO_THROW(ValidateNoChildren(tree));
  TreeData child{CHILD_ELEMENT_NAME};
  tree.AddChild(child);
  EXPECT_THROW(ValidateNoChildren(tree), ValidationException);
}

TEST_F(TreeDataValidateTest, NoContent)
{
  TreeData tree{ROOT_ELEMENT_NAME};
  EXPECT_NO_THROW(ValidateNoContent(tree));
  tree.SetContent("ContentNotAllowed");
  EXPECT_THROW(ValidateNoContent(tree), ValidationException);
}

TreeDataValidateTest::TreeDataValidateTest() = default;

TreeDataValidateTest::~TreeDataValidateTest() = default;
