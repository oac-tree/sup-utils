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

#include <sup/xml/exceptions.h>
#include <sup/xml/tree_data_parser.h>
#include <sup/xml/tree_data_serialize.h>

#include <gtest/gtest.h>

using namespace sup::xml;

static const std::string MEMBERLIST_NODE = "MemberList";
static const std::string MEMBER_NODE = "Member";
static const std::string NAME_NODE = "Name";
static const std::string DETAILS_NODE = "Details";

static const std::string KEY_ATTRIBUTE = "key";
static const std::string FORMAT_ATTRIBUTE = "format";
static const std::string COUNTRY_ATTRIBUTE = "country";
static const std::string MEMBERSHIP_ATTRIBUTE = "membership";

static const std::string XML_REPR_BODY = R"RAW(
<MemberList>
  <Member key="433">
    <Name format="full">Martha Thompson</Name>
    <Details country="FR" membership="gold"/>
  </Member>
  <Member key="23">
    <Name format="prename">Anna</Name>
    <Details country="DE" membership="platina"/>
  </Member>
</MemberList>
)RAW";

class TreeDataSerializeTest : public ::testing::Test
{
protected:
  TreeDataSerializeTest();
  virtual ~TreeDataSerializeTest();

  std::string AddXMLHeader(const std::string& body);

  TreeData m_tree;
};

TEST_F(TreeDataSerializeTest, ToString)
{
  // Serialize to XML string
  auto xml_repr = TreeDataToString(m_tree);
  EXPECT_EQ(xml_repr, AddXMLHeader(XML_REPR_BODY));
}

TEST_F(TreeDataSerializeTest, ToFile)
{
  // Serialize to XML file
  std::string filename = "memberlist_test";
  sup::unit_test_helper::TemporaryTestFile(filename, "");
  EXPECT_NO_THROW(TreeDataToFile(filename, m_tree));

  // Read back from file
  auto tree = TreeDataFromFile(filename);
  EXPECT_EQ(*tree, m_tree);
}

TreeDataSerializeTest::TreeDataSerializeTest()
  : m_tree{MEMBERLIST_NODE}
{
  TreeData martha{MEMBER_NODE};
  martha.AddAttribute(KEY_ATTRIBUTE, "433");
  TreeData martha_name{NAME_NODE};
  martha_name.AddAttribute(FORMAT_ATTRIBUTE, "full");
  martha_name.SetContent("Martha Thompson");
  martha.AddChild(martha_name);
  TreeData martha_details{DETAILS_NODE};
  martha_details.AddAttribute(COUNTRY_ATTRIBUTE, "FR");
  martha_details.AddAttribute(MEMBERSHIP_ATTRIBUTE, "gold");
  martha.AddChild(martha_details);
  m_tree.AddChild(martha);
  TreeData anna{MEMBER_NODE};
  anna.AddAttribute(KEY_ATTRIBUTE, "23");
  TreeData anna_name{NAME_NODE};
  anna_name.AddAttribute(FORMAT_ATTRIBUTE, "prename");
  anna_name.SetContent("Anna");
  anna.AddChild(anna_name);
  TreeData anna_details{DETAILS_NODE};
  anna_details.AddAttribute(COUNTRY_ATTRIBUTE, "DE");
  anna_details.AddAttribute(MEMBERSHIP_ATTRIBUTE, "platina");
  anna.AddChild(anna_details);
  m_tree.AddChild(anna);
}

TreeDataSerializeTest::~TreeDataSerializeTest() = default;

std::string TreeDataSerializeTest::AddXMLHeader(const std::string& body)
{
  static const std::string header{R"RAW(<?xml version="1.0" encoding="UTF-8"?>)RAW"};
  return header + body;
}
