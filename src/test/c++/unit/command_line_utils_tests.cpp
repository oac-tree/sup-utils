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

TEST_F(CommandLineUtilsTests, GetSummaryString)
{
  std::vector<const CommandLineOption *> options;
  EXPECT_EQ(GetSummaryString(options), std::string("[options]"));
}
