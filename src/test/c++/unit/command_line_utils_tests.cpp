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
#include <sup/cli/command_line_option.h>
#include <sup/cli/command_line_utils.h>

using namespace sup::cli;

class CommandLineUtilsTests : public ::testing::Test
{
};

//! Testing GetAvailableOptionsSummaryString function.

TEST_F(CommandLineUtilsTests, GetAvailableOptionsSummaryString)
{
  std::vector<const CommandLineOption *> options;
  EXPECT_EQ(GetAvailableOptionsSummaryString(options), std::string("[options]"));
}

//! Testing GetOptionUsageString function.

TEST_F(CommandLineUtilsTests, GetOptionUsageString)
{
  EXPECT_TRUE(GetOptionUsageString(CommandLineOption({})).empty());

  // flags
  EXPECT_EQ(GetOptionUsageString(CommandLineOption({"-v"})), std::string("-v"));
  EXPECT_EQ(GetOptionUsageString(CommandLineOption({"-v", "--verbose"})),
            std::string("-v, --verbose"));

  {  // unnamed parameter
    CommandLineOption option({"-f", "--font"});
    option.SetParameter(true);
    EXPECT_EQ(GetOptionUsageString(option), std::string("-f, --font <value>"));
  }

  {  // named parameter
    CommandLineOption option({"-f", "--font"});
    option.SetParameter(true)->SetValueName("size");
    EXPECT_EQ(GetOptionUsageString(option), std::string("-f, --font <size>"));
  }
}

//! Testing GetUsageString function.

TEST_F(CommandLineUtilsTests, GetUsageString)
{
  {  // no options
    std::vector<const CommandLineOption *> options;
    std::string expected(R"RAW(Usage: myprogram [options]

)RAW");
    EXPECT_EQ(GetUsageString("myprogram", options), expected);
  }

  {  // single flag
    CommandLineOption option({"-v", "--verbose"});
    option.SetDescription("description");
    std::vector<const CommandLineOption *> options{&option};
    std::string expected(R"RAW(Usage: myprogram [options]

Options:
-v, --verbose       description
)RAW");
    EXPECT_EQ(GetUsageString("myprogram", options), expected);
  }
}
