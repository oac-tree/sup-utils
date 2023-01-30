/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : Supervision and Automation System Utilities
 *
 * Description   : SUP logging
 *
 * Author        : Walter Van Herck (IO)
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

using namespace sup::cli;

class CommandLineOptionTests : public ::testing::Test
{
};

TEST_F(CommandLineOptionTests, Construction)
{
  CommandLineOption option1({"-f"});
  EXPECT_EQ(option1.GetOptionNames(), std::vector<std::string>({"-f"}));

  CommandLineOption option2({"-f", "--file"});
  EXPECT_EQ(option2.GetOptionNames(), std::vector<std::string>({"-f", "--file"}));
}

TEST_F(CommandLineOptionTests, FluentInterface)
{
  CommandLineOption option({"-f"});
  option.SetDefaultValue("abc")->SetDescription("description");

  EXPECT_EQ(option.GetDefaultValue(), std::string("abc"));
  EXPECT_EQ(option.GetDescription(), std::string("description"));
}
