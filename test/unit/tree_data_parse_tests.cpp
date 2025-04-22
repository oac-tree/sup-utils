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

#include "unit_test_helper.h"

#include <sup/xml/exceptions.h>
#include <sup/xml/tree_data_parser.h>
#include <sup/xml/tree_data_parser_utils.h>
#include <sup/xml/tree_data_serialize.h>

#include <gtest/gtest.h>

using namespace sup::xml;

//! Check if content string is empty apart from newlines and spaces
static bool ContentEmpty(const std::string& content);

class TreeDataParserTest : public ::testing::Test
{
protected:
  TreeDataParserTest();
  virtual ~TreeDataParserTest();

  std::string AddXMLHeader(const std::string& body);
};

TEST_F(TreeDataParserTest, FromString)
{
  std::string body = R"RAW(
    <MemberList>
      <Member key="433">
        <Name format="full">Martha Thompson</Name>
        <PhoneNumber>12345</PhoneNumber>
        <Details country="FR" membership="gold"/>
      </Member>
      <Member key="23">
        <Name format="prename">Anna</Name>
        <Details country="DE" membership="platina"/>
      </Member>
    </MemberList>
  )RAW";

  // Parse XML from string
  auto xml_str = AddXMLHeader(body);
  auto tree_data = TreeDataFromString(xml_str);
  ASSERT_TRUE(static_cast<bool>(tree_data));

  // Inspect root node
  EXPECT_EQ(tree_data->GetNodeName(), "MemberList");
  EXPECT_EQ(tree_data->GetNumberOfAttributes(), 0);
  EXPECT_EQ(tree_data->GetNumberOfChildren(), 2);
  EXPECT_TRUE(ContentEmpty(tree_data->GetContent()));
  auto& children = tree_data->Children();
  EXPECT_EQ(children.size(), 2);

  // Inspect first child node
  auto member1 = children[0];
  EXPECT_EQ(member1.GetNodeName(), "Member");
  EXPECT_EQ(member1.GetNumberOfAttributes(), 1);
  EXPECT_TRUE(member1.HasAttribute("key"));
  EXPECT_EQ(member1.GetAttribute("key"), "433");
  EXPECT_EQ(member1.GetNumberOfChildren(), 3);
  EXPECT_TRUE(ContentEmpty(member1.GetContent()));
  auto& mem1_children = member1.Children();
  EXPECT_EQ(mem1_children.size(), 3);
  // Inspect first child's Name node
  auto mem1_name = mem1_children[0];
  EXPECT_EQ(mem1_name.GetNodeName(), "Name");
  EXPECT_EQ(mem1_name.GetNumberOfAttributes(), 1);
  EXPECT_TRUE(mem1_name.HasAttribute("format"));
  EXPECT_EQ(mem1_name.GetAttribute("format"), "full");
  EXPECT_EQ(mem1_name.GetNumberOfChildren(), 0);
  EXPECT_EQ(mem1_name.GetContent(), "Martha Thompson");
  // Inspect first child's PhoneNumber node
  auto mem1_phone = mem1_children[1];
  EXPECT_EQ(mem1_phone.GetNodeName(), "PhoneNumber");
  EXPECT_EQ(mem1_phone.GetNumberOfAttributes(), 0);
  EXPECT_EQ(mem1_phone.GetNumberOfChildren(), 0);
  EXPECT_EQ(mem1_phone.GetContent(), "12345");
  // Inspect first child's Details node
  auto mem1_details = mem1_children[2];
  EXPECT_EQ(mem1_details.GetNodeName(), "Details");
  EXPECT_EQ(mem1_details.GetNumberOfAttributes(), 2);
  EXPECT_TRUE(mem1_details.HasAttribute("country"));
  EXPECT_EQ(mem1_details.GetAttribute("country"), "FR");
  EXPECT_TRUE(mem1_details.HasAttribute("membership"));
  EXPECT_EQ(mem1_details.GetAttribute("membership"), "gold");
  EXPECT_EQ(mem1_details.GetNumberOfChildren(), 0);
  EXPECT_TRUE(ContentEmpty(mem1_details.GetContent()));

  // Inspect second child node
  auto member2 = children[1];
  EXPECT_EQ(member2.GetNodeName(), "Member");
  EXPECT_EQ(member2.GetNumberOfAttributes(), 1);
  EXPECT_TRUE(member2.HasAttribute("key"));
  EXPECT_EQ(member2.GetAttribute("key"), "23");
  EXPECT_EQ(member2.GetNumberOfChildren(), 2);
  EXPECT_TRUE(ContentEmpty(member2.GetContent()));
  auto& mem2_children = member2.Children();
  EXPECT_EQ(mem2_children.size(), 2);
  // Inspect second child's Name node
  auto mem2_name = mem2_children[0];
  EXPECT_EQ(mem2_name.GetNodeName(), "Name");
  EXPECT_EQ(mem2_name.GetNumberOfAttributes(), 1);
  EXPECT_TRUE(mem2_name.HasAttribute("format"));
  EXPECT_EQ(mem2_name.GetAttribute("format"), "prename");
  EXPECT_EQ(mem2_name.GetNumberOfChildren(), 0);
  EXPECT_EQ(mem2_name.GetContent(), "Anna");
  // Inspect second child's Details node
  auto mem2_details = mem2_children[1];
  EXPECT_EQ(mem2_details.GetNodeName(), "Details");
  EXPECT_EQ(mem2_details.GetNumberOfAttributes(), 2);
  EXPECT_TRUE(mem2_details.HasAttribute("country"));
  EXPECT_EQ(mem2_details.GetAttribute("country"), "DE");
  EXPECT_TRUE(mem2_details.HasAttribute("membership"));
  EXPECT_EQ(mem2_details.GetAttribute("membership"), "platina");
  EXPECT_EQ(mem2_details.GetNumberOfChildren(), 0);
  EXPECT_TRUE(ContentEmpty(mem2_details.GetContent()));
}

TEST_F(TreeDataParserTest, FromFile)
{
  std::string body = R"RAW(
    <MemberList>
      <Member key="433">
        <Name format="full">Martha Thompson</Name>
        <PhoneNumber>12345</PhoneNumber>
        <Details country="FR" membership="gold"/>
      </Member>
      <Member key="23">
        <Name format="prename">Anna</Name>
        <Details country="DE" membership="platina"/>
      </Member>
    </MemberList>
  )RAW";
  const std::string filename = "TreeDataParserTest_FromFile";
  auto xml_str = AddXMLHeader(body);
  sup::unit_test_helper::TemporaryTestFile xml_file(filename, xml_str);

  // Parse XML from file
  auto tree_data = TreeDataFromFile(filename);
  ASSERT_TRUE(static_cast<bool>(tree_data));

  // Inspect root node
  EXPECT_EQ(tree_data->GetNodeName(), "MemberList");
  EXPECT_EQ(tree_data->GetNumberOfAttributes(), 0);
  EXPECT_EQ(tree_data->GetNumberOfChildren(), 2);
  EXPECT_TRUE(ContentEmpty(tree_data->GetContent()));
  auto& children = tree_data->Children();
  EXPECT_EQ(children.size(), 2);

  // Inspect first child node
  auto member1 = children[0];
  EXPECT_EQ(member1.GetNodeName(), "Member");
  EXPECT_EQ(member1.GetNumberOfAttributes(), 1);
  EXPECT_TRUE(member1.HasAttribute("key"));
  EXPECT_EQ(member1.GetAttribute("key"), "433");
  EXPECT_EQ(member1.GetNumberOfChildren(), 3);
  EXPECT_TRUE(ContentEmpty(member1.GetContent()));
  auto& mem1_children = member1.Children();
  EXPECT_EQ(mem1_children.size(), 3);
  // Inspect first child's Name node
  auto mem1_name = mem1_children[0];
  EXPECT_EQ(mem1_name.GetNodeName(), "Name");
  EXPECT_EQ(mem1_name.GetNumberOfAttributes(), 1);
  EXPECT_TRUE(mem1_name.HasAttribute("format"));
  EXPECT_EQ(mem1_name.GetAttribute("format"), "full");
  EXPECT_EQ(mem1_name.GetNumberOfChildren(), 0);
  EXPECT_EQ(mem1_name.GetContent(), "Martha Thompson");
  // Inspect first child's PhoneNumber node
  auto mem1_phone = mem1_children[1];
  EXPECT_EQ(mem1_phone.GetNodeName(), "PhoneNumber");
  EXPECT_EQ(mem1_phone.GetNumberOfAttributes(), 0);
  EXPECT_EQ(mem1_phone.GetNumberOfChildren(), 0);
  EXPECT_EQ(mem1_phone.GetContent(), "12345");
  // Inspect first child's Details node
  auto mem1_details = mem1_children[2];
  EXPECT_EQ(mem1_details.GetNodeName(), "Details");
  EXPECT_EQ(mem1_details.GetNumberOfAttributes(), 2);
  EXPECT_TRUE(mem1_details.HasAttribute("country"));
  EXPECT_EQ(mem1_details.GetAttribute("country"), "FR");
  EXPECT_TRUE(mem1_details.HasAttribute("membership"));
  EXPECT_EQ(mem1_details.GetAttribute("membership"), "gold");
  EXPECT_EQ(mem1_details.GetNumberOfChildren(), 0);
  EXPECT_TRUE(ContentEmpty(mem1_details.GetContent()));

  // Inspect second child node
  auto member2 = children[1];
  EXPECT_EQ(member2.GetNodeName(), "Member");
  EXPECT_EQ(member2.GetNumberOfAttributes(), 1);
  EXPECT_TRUE(member2.HasAttribute("key"));
  EXPECT_EQ(member2.GetAttribute("key"), "23");
  EXPECT_EQ(member2.GetNumberOfChildren(), 2);
  EXPECT_TRUE(ContentEmpty(member2.GetContent()));
  auto& mem2_children = member2.Children();
  EXPECT_EQ(mem2_children.size(), 2);
  // Inspect second child's Name node
  auto mem2_name = mem2_children[0];
  EXPECT_EQ(mem2_name.GetNodeName(), "Name");
  EXPECT_EQ(mem2_name.GetNumberOfAttributes(), 1);
  EXPECT_TRUE(mem2_name.HasAttribute("format"));
  EXPECT_EQ(mem2_name.GetAttribute("format"), "prename");
  EXPECT_EQ(mem2_name.GetNumberOfChildren(), 0);
  EXPECT_EQ(mem2_name.GetContent(), "Anna");
  // Inspect second child's Details node
  auto mem2_details = mem2_children[1];
  EXPECT_EQ(mem2_details.GetNodeName(), "Details");
  EXPECT_EQ(mem2_details.GetNumberOfAttributes(), 2);
  EXPECT_TRUE(mem2_details.HasAttribute("country"));
  EXPECT_EQ(mem2_details.GetAttribute("country"), "DE");
  EXPECT_TRUE(mem2_details.HasAttribute("membership"));
  EXPECT_EQ(mem2_details.GetAttribute("membership"), "platina");
  EXPECT_EQ(mem2_details.GetNumberOfChildren(), 0);
  EXPECT_TRUE(ContentEmpty(mem2_details.GetContent()));
}

TEST_F(TreeDataParserTest, FromStringError)
{
  std::string wrong_xml = R"RAW(
    <MemberList>
  )RAW";
  EXPECT_THROW(TreeDataFromString(wrong_xml), ParseException);

  std::string empty_xml = AddXMLHeader("");
  EXPECT_THROW(TreeDataFromString(empty_xml), ParseException);
}

TEST_F(TreeDataParserTest, FromFileError)
{
  const std::string non_existing_filename = "File_does_not_exist";
  EXPECT_THROW(TreeDataFromFile(non_existing_filename), ParseException);

  std::string wrong_xml = R"RAW(
    <MemberList>
  )RAW";
  const std::string bad_xml_filename = "TreeDataParserTest_BadXML";
  sup::unit_test_helper::TemporaryTestFile xml_file(bad_xml_filename, wrong_xml);
  EXPECT_THROW(TreeDataFromFile(bad_xml_filename), ParseException);
}

TEST_F(TreeDataParserTest, FromStringAndRoundtrip)
{
  std::string body = R"RAW(
    <MemberList>
      <Member key="433">
        <Name format="full">Martha Thompson</Name>
        <PhoneNumber>12345</PhoneNumber>
        <Details country="FR" membership="gold"/>
      </Member>
      <Member key="23">
        <Name format="prename">Anna</Name>
        <Details country="DE" membership="platina"/>
      </Member>
    </MemberList>
  )RAW";

  // Parse XML from string
  auto xml_str = AddXMLHeader(body);
  auto tree_data = TreeDataFromString(xml_str);
  ASSERT_TRUE(static_cast<bool>(tree_data));

  // Serialize and parse again
  auto tree_data_roundtrip = TreeDataFromString(TreeDataToString(*tree_data));
  ASSERT_TRUE(static_cast<bool>(tree_data_roundtrip));
  EXPECT_EQ(*tree_data_roundtrip, *tree_data);
}

TEST_F(TreeDataParserTest, DepthFour)
{
  std::string body = R"RAW(
    <MemberList>
      <Group name="patrons">
        <Member key="433">
          <Name format="full">Martha Thompson</Name>
        </Member>
        <Member key="23">
          <Name format="prename">Anna</Name>
        </Member>
      </Group>
    </MemberList>
  )RAW";

  // Parse XML from string
  auto xml_str = AddXMLHeader(body);
  auto tree_data = TreeDataFromString(xml_str);
  ASSERT_TRUE(static_cast<bool>(tree_data));

  // Inspect root node
  EXPECT_EQ(tree_data->GetNodeName(), "MemberList");
  EXPECT_EQ(tree_data->GetNumberOfAttributes(), 0);
  EXPECT_EQ(tree_data->GetNumberOfChildren(), 1);
  EXPECT_TRUE(ContentEmpty(tree_data->GetContent()));
  auto& root_children = tree_data->Children();
  ASSERT_EQ(root_children.size(), 1);

  // Inspect Group node
  auto group_node = root_children[0];
  EXPECT_EQ(group_node.GetNodeName(), "Group");
  EXPECT_EQ(group_node.GetNumberOfAttributes(), 1);
  EXPECT_TRUE(group_node.HasAttribute("name"));
  EXPECT_EQ(group_node.GetAttribute("name"), "patrons");
  EXPECT_EQ(group_node.GetNumberOfChildren(), 2);
  EXPECT_TRUE(ContentEmpty(group_node.GetContent()));
  auto& children = group_node.Children();
  ASSERT_EQ(children.size(), 2);

  // Inspect first child node
  auto member1 = children[0];
  EXPECT_EQ(member1.GetNodeName(), "Member");
  EXPECT_EQ(member1.GetNumberOfAttributes(), 1);
  EXPECT_TRUE(member1.HasAttribute("key"));
  EXPECT_EQ(member1.GetAttribute("key"), "433");
  EXPECT_EQ(member1.GetNumberOfChildren(), 1);
  EXPECT_TRUE(ContentEmpty(member1.GetContent()));
  auto& mem1_children = member1.Children();
  ASSERT_EQ(mem1_children.size(), 1);
  // Inspect first child's Name node
  auto mem1_name = mem1_children[0];
  EXPECT_EQ(mem1_name.GetNodeName(), "Name");
  EXPECT_EQ(mem1_name.GetNumberOfAttributes(), 1);
  EXPECT_TRUE(mem1_name.HasAttribute("format"));
  EXPECT_EQ(mem1_name.GetAttribute("format"), "full");
  EXPECT_EQ(mem1_name.GetNumberOfChildren(), 0);
  EXPECT_EQ(mem1_name.GetContent(), "Martha Thompson");

  // Inspect second child node
  auto member2 = children[1];
  EXPECT_EQ(member2.GetNodeName(), "Member");
  EXPECT_EQ(member2.GetNumberOfAttributes(), 1);
  EXPECT_TRUE(member2.HasAttribute("key"));
  EXPECT_EQ(member2.GetAttribute("key"), "23");
  EXPECT_EQ(member2.GetNumberOfChildren(), 1);
  EXPECT_TRUE(ContentEmpty(member2.GetContent()));
  auto& mem2_children = member2.Children();
  ASSERT_EQ(mem2_children.size(), 1);
  // Inspect second child's Name node
  auto mem2_name = mem2_children[0];
  EXPECT_EQ(mem2_name.GetNodeName(), "Name");
  EXPECT_EQ(mem2_name.GetNumberOfAttributes(), 1);
  EXPECT_TRUE(mem2_name.HasAttribute("format"));
  EXPECT_EQ(mem2_name.GetAttribute("format"), "prename");
  EXPECT_EQ(mem2_name.GetNumberOfChildren(), 0);
  EXPECT_EQ(mem2_name.GetContent(), "Anna");
}

TEST_F(TreeDataParserTest, ParseExceptions)
{
  EXPECT_THROW(ParseXMLDoc(nullptr), ParseException);
}

static bool ContentEmpty(const std::string& content)
{
  return content.find_first_not_of("\n ") == std::string::npos;
}

TreeDataParserTest::TreeDataParserTest() = default;

TreeDataParserTest::~TreeDataParserTest() = default;

std::string TreeDataParserTest::AddXMLHeader(const std::string& body)
{
  static const std::string header{R"RAW(<?xml version="1.0" encoding="UTF-8"?>)RAW"};
  return header + body;
}
