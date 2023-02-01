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

TEST_F(CommandLineParserTests, AddHelpOption)
{
  CommandLineParser parser;
  auto option = parser.AddHelpOption();

  ASSERT_TRUE(option != nullptr);
  EXPECT_EQ(option->GetOptionNames(), std::vector<std::string>({"-h", "--help"}));
  EXPECT_FALSE(option->IsParameter());
  EXPECT_FALSE(option->IsPositional());
}

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

//! Parsing a single option without a parameter (a flag).

TEST_F(CommandLineParserTests, ParseFlag)
{
  CommandLineParser parser;
  auto option = parser.AddOption({"--verbose"});

  EXPECT_FALSE(option->IsParameter());
  EXPECT_FALSE(option->IsPositional());

  const int argc = 2;
  std::array<const char *, argc> argv{"progname", "--verbose"};

  EXPECT_TRUE(parser.Parse(argc, &argv[0]));

  EXPECT_TRUE(parser.IsSet("--verbose"));
  EXPECT_FALSE(parser.IsSet("-f"));
}

//! Parsing two options without a parameter (flags).

TEST_F(CommandLineParserTests, ParseTwoFlags)
{
  CommandLineParser parser;
  parser.AddOption({"-v", "--verbose"});
  parser.AddOption({"-f", "--file"});

  const int argc = 3;
  // command line contains short version of flags
  std::array<const char *, argc> argv{"progname", "-v", "-f"};

  EXPECT_TRUE(parser.Parse(argc, &argv[0]));

  // parser should report both versions (short and long) as set
  EXPECT_TRUE(parser.IsSet("--verbose"));
  EXPECT_TRUE(parser.IsSet("-v"));
  EXPECT_TRUE(parser.IsSet("--file"));
  EXPECT_TRUE(parser.IsSet("-f"));
}

//! Parsing single parameter (option that has a value).

TEST_F(CommandLineParserTests, ParseParameter)
{
  CommandLineParser parser;
  auto option = parser.AddOption({"--font"})->SetParameter(true);

  EXPECT_TRUE(option->IsParameter());
  EXPECT_FALSE(option->IsPositional());

  const int argc = 2;
  // command line contains short version of flags
  std::array<const char *, argc> argv{"progname", "--font=10"};

  EXPECT_TRUE(parser.Parse(argc, &argv[0]));

  EXPECT_TRUE(parser.IsSet("--font"));

  EXPECT_EQ(parser.GetValue<int>("--font"), 10);
}

//! Parsing command line string containing a help option.

TEST_F(CommandLineParserTests, ParseHelpOption)
{
  CommandLineParser parser;
  parser.AddHelpOption();

  const int argc = 2;
  std::array<const char *, argc> argv{"progname", "--help"};

  EXPECT_FALSE(parser.Parse(argc, &argv[0]));
}

//! Validate multiline string representing `usage` help for the single option setup.

TEST_F(CommandLineParserTests, GetUsageString)
{
  CommandLineParser parser;
  auto option = parser.AddOption({"-v", "--verbose"});
  option->SetDescription("description");

  const int argc = 2;
  // command line contains short version of flags
  std::array<const char *, argc> argv{"progname", "--verbose"};

  EXPECT_TRUE(parser.Parse(argc, &argv[0]));

  std::string expected(R"RAW(Usage: progname [options]

Options:
-v, --verbose       description
)RAW");
  EXPECT_EQ(parser.GetUsageString(), expected);
}
