/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : Supervision and Automation System Utilities
 *
 * Description   : Command line argument parsing
 *
 * Author        : Gennady Pospelov (IO)
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

#include <gtest/gtest.h>

#include <sup/cli/command_line_parser.h>

using namespace sup::cli;

class CommandLineParserTests : public ::testing::Test
{
};

TEST_F(CommandLineParserTests, AddOption)
{
  CommandLineParser parser;
  auto option1 = parser.AddOption({"-f", "--file"});
  ASSERT_TRUE(option1 != nullptr);
  EXPECT_EQ(option1->GetOptionNames(), std::vector<std::string>({"-f", "--file"}));

  auto option2 = parser.AddOption({"--help"});
  ASSERT_TRUE(option2 != nullptr);
  EXPECT_EQ(option2->GetOptionNames(), std::vector<std::string>({"--help"}));

  // access to existing options
  EXPECT_EQ(parser.GetOption("-f"), option1);
  EXPECT_EQ(parser.GetOption("--file"), option1);
  EXPECT_EQ(parser.GetOption("--help"), option2);

  // non-existing options
  EXPECT_EQ(parser.GetOption("-file"), nullptr);
  EXPECT_EQ(parser.GetOption("-help"), nullptr);
  EXPECT_EQ(parser.GetOption("-nonexisting"), nullptr);
}

TEST_F(CommandLineParserTests, FluentInterface)
{
  CommandLineParser parser;
  auto option =
      parser.AddOption({"-f", "--file"})->SetDescription("description")->SetDefaultValue("value");

  EXPECT_EQ(option->GetOptionNames(), std::vector<std::string>({"-f", "--file"}));
  EXPECT_EQ(option->GetDescription(), std::string("description"));
  EXPECT_EQ(option->GetDefaultValue(), std::string("value"));
}
