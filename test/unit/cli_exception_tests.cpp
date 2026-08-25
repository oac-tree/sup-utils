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

#include <sup/cli/exceptions.h>

#include <gtest/gtest.h>

#include <stdexcept>
#include <string>
#include <type_traits>

using namespace sup::cli;

class CommandLineExceptionTests : public ::testing::Test
{
};

TEST_F(CommandLineExceptionTests, MessageIsCarried)
{
  const std::string message{"message"};

  EXPECT_EQ(std::string(CommandLineException{message}.what()), message);
  EXPECT_EQ(std::string(InvalidOptionException{message}.what()), message);
  EXPECT_EQ(std::string(MissingValueException{message}.what()), message);
}

//! Every exception of this library is a std::runtime_error, which is what the parser used to throw
//! for any failure. Applications catching that keep working.
TEST_F(CommandLineExceptionTests, Hierarchy)
{
  static_assert(std::is_base_of_v<std::runtime_error, CommandLineException>,
                "applications catching std::runtime_error shall keep working");
  static_assert(std::is_base_of_v<CommandLineException, InvalidOptionException>);
  static_assert(std::is_base_of_v<CommandLineException, MissingValueException>);

  // the two are unrelated to each other, so neither is ever caught in place of the other
  static_assert(!std::is_base_of_v<InvalidOptionException, MissingValueException>);
  static_assert(!std::is_base_of_v<MissingValueException, InvalidOptionException>);

  const std::string message{"message"};
  EXPECT_THROW(throw InvalidOptionException{message}, std::runtime_error);
  EXPECT_THROW(throw MissingValueException{message}, std::runtime_error);
}
