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

#include <sup/cli/command_line_option.h>
#include <sup/cli/command_line_utils.h>

#include <gtest/gtest.h>

using namespace sup::cli;

class CommandLineUtilsTests : public ::testing::Test
{
};

//! Testing GetFilteredOptions helper method
TEST_F(CommandLineUtilsTests, GetFilteredOptions)
{
  CommandLineOption option1({"abc"});
  CommandLineOption option2({"def"});
  option2.SetPositional(true);
  std::vector<const CommandLineOption *> options({&option1, &option2});

  const std::vector<const CommandLineOption *> expected_non_positional_options({&option1});
  const std::vector<const CommandLineOption *> expected_positional_options({&option2});

  EXPECT_EQ(GetFilteredOptions(options, false), expected_non_positional_options);
  EXPECT_EQ(GetFilteredOptions(options, true), expected_positional_options);
}

//! Testing MergeWithNewLine function.
TEST_F(CommandLineUtilsTests, MergeWithNewLine)
{
  EXPECT_EQ(MergeWithNewLine({"abc"}), std::string("abc\n"));
  EXPECT_EQ(MergeWithNewLine({"abc", "efg"}), std::string("abc\nefg\n"));
}

//! Testing GetAvailableOptionsSummaryString function.
TEST_F(CommandLineUtilsTests, GetAvailableOptionsSummaryString)
{
  std::vector<const CommandLineOption *> options;
  EXPECT_EQ(GetAvailableOptionsSummaryString(options), std::string("[options]"));
}

//! Testing GetOptionNameString function.
TEST_F(CommandLineUtilsTests, GetOptionNameString)
{
  EXPECT_TRUE(GetOptionNameString(CommandLineOption({})).empty());

  // flags
  EXPECT_EQ(GetOptionNameString(CommandLineOption({"-v"})), std::string("-v"));
  EXPECT_EQ(GetOptionNameString(CommandLineOption({"-v", "--verbose"})),
            std::string("-v, --verbose"));

  {  // unnamed parameter
    CommandLineOption option({"-f", "--font"});
    option.SetParameter(true);
    EXPECT_EQ(GetOptionNameString(option), std::string("-f, --font <value>"));
  }

  {  // named parameter
    CommandLineOption option({"-f", "--font"});
    option.SetParameter(true).SetValueName("size");
    EXPECT_EQ(GetOptionNameString(option), std::string("-f, --font <size>"));
  }
}

//! Testing GetUsageString function.
TEST_F(CommandLineUtilsTests, GetOptionBlockString)
{
  EXPECT_TRUE(GetOptionBlockString(std::vector<const CommandLineOption *>()).empty());

  CommandLineOption option1({"-v", "--verbose"});
  option1.SetDescription("description");

  CommandLineOption option2({"--help"});
  option2.SetDescription("help");

  std::vector<const CommandLineOption *> options{&option1, &option2};
  std::string expected(R"RAW(Options:
-v, --verbose                 description
--help                        help
)RAW");
  EXPECT_EQ(GetOptionBlockString(options), expected);
}

//! Testing GetUsageString function.
TEST_F(CommandLineUtilsTests, GetUsageString)
{
  {  // no options
    std::vector<const CommandLineOption *> options;
    std::string expected(R"RAW(Usage: myprogram [options]

)RAW");
    EXPECT_EQ(GetUsageString("myprogram", "", options, ""), expected);
  }

  {  // single flag
    CommandLineOption option({"-v", "--verbose"});
    option.SetDescription("description");
    std::vector<const CommandLineOption *> options{&option};
    std::string expected(R"RAW(Usage: myprogram [options]

Options:
-v, --verbose                 description

)RAW");
    EXPECT_EQ(GetUsageString("myprogram", "", options, ""), expected);
  }
}
