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
#include <sup/cli/value_conversion.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdint>
#include <limits>
#include <string>

using namespace sup::cli;

class ValueConversionTests : public ::testing::Test
{
};

TEST_F(ValueConversionTests, ConvertToString)
{
  // the text is returned as it was given, whitespace and all
  EXPECT_EQ(ConvertValue<std::string>("context", "abc"), std::string("abc"));
  EXPECT_EQ(ConvertValue<std::string>("context", "abc def"), std::string("abc def"));
  EXPECT_EQ(ConvertValue<std::string>("context", " "), std::string(" "));
  EXPECT_EQ(ConvertValue<std::string>("context", ""), std::string(""));
}

TEST_F(ValueConversionTests, ConvertToSignedInteger)
{
  EXPECT_EQ(ConvertValue<int>("context", "42"), 42);
  EXPECT_EQ(ConvertValue<int>("context", "-42"), -42);
  EXPECT_EQ(ConvertValue<int>("context", "+42"), 42);
  EXPECT_EQ(ConvertValue<int>("context", "0"), 0);
  EXPECT_EQ(ConvertValue<std::int64_t>("context", "9223372036854775807"),
            std::numeric_limits<std::int64_t>::max());

  // the whole text is converted, or nothing is
  EXPECT_THROW(ConvertValue<int>("context", "64k"), InvalidValueException);
  EXPECT_THROW(ConvertValue<int>("context", "0x10"), InvalidValueException);
  EXPECT_THROW(ConvertValue<int>("context", "42 "), InvalidValueException);
  EXPECT_THROW(ConvertValue<int>("context", " 42"), InvalidValueException);
  EXPECT_THROW(ConvertValue<int>("context", ""), InvalidValueException);
  EXPECT_THROW(ConvertValue<int>("context", "abc"), InvalidValueException);
  EXPECT_THROW(ConvertValue<int>("context", "4.2"), InvalidValueException);

  // a number too large for the requested type is reported, not truncated
  EXPECT_THROW(ConvertValue<std::int16_t>("context", "40000"), InvalidValueException);
  EXPECT_THROW(ConvertValue<std::int8_t>("context", "-200"), InvalidValueException);
  EXPECT_THROW(ConvertValue<std::int64_t>("context", "9223372036854775808"), InvalidValueException);
}

TEST_F(ValueConversionTests, ConvertToUnsignedInteger)
{
  EXPECT_EQ(ConvertValue<unsigned int>("context", "42"), 42U);
  EXPECT_EQ(ConvertValue<std::uint8_t>("context", "255"), 255U);
  EXPECT_EQ(ConvertValue<std::uint64_t>("context", "18446744073709551615"),
            std::numeric_limits<std::uint64_t>::max());

  // a negative number does not wrap around to a large positive one
  EXPECT_THROW(ConvertValue<unsigned int>("context", "-1"), InvalidValueException);
  EXPECT_THROW(ConvertValue<std::uint64_t>("context", "-1"), InvalidValueException);

  EXPECT_THROW(ConvertValue<std::uint8_t>("context", "300"), InvalidValueException);
  EXPECT_THROW(ConvertValue<unsigned int>("context", "12ab"), InvalidValueException);
}

TEST_F(ValueConversionTests, ConvertToFloatingPoint)
{
  EXPECT_DOUBLE_EQ(ConvertValue<double>("context", "0.5"), 0.5);
  EXPECT_DOUBLE_EQ(ConvertValue<double>("context", "1e4"), 10000.0);
  EXPECT_DOUBLE_EQ(ConvertValue<double>("context", "-2.5"), -2.5);
  EXPECT_DOUBLE_EQ(ConvertValue<double>("context", "42"), 42.0);
  EXPECT_FLOAT_EQ(ConvertValue<float>("context", "0.5"), 0.5F);

  EXPECT_THROW(ConvertValue<double>("context", "1.5.5"), InvalidValueException);
  EXPECT_THROW(ConvertValue<double>("context", "0.5abc"), InvalidValueException);
  EXPECT_THROW(ConvertValue<double>("context", ""), InvalidValueException);

  // a double beyond what a float can hold
  EXPECT_THROW(ConvertValue<float>("context", "1e40"), InvalidValueException);
}

TEST_F(ValueConversionTests, ConvertToBoolean)
{
  EXPECT_TRUE(ConvertValue<bool>("context", "1"));
  EXPECT_TRUE(ConvertValue<bool>("context", "true"));
  EXPECT_FALSE(ConvertValue<bool>("context", "0"));
  EXPECT_FALSE(ConvertValue<bool>("context", "false"));

  EXPECT_THROW(ConvertValue<bool>("context", "yes"), InvalidValueException);
  EXPECT_THROW(ConvertValue<bool>("context", "2"), InvalidValueException);
  EXPECT_THROW(ConvertValue<bool>("context", ""), InvalidValueException);
}

//! The message names what the value belongs to, and what is wrong with it.
TEST_F(ValueConversionTests, MessageNamesTheValueAndTheContext)
{
  EXPECT_THAT([] { (void)ConvertValue<int>("option \'--size\'", "64k"); },
              ::testing::ThrowsMessage<InvalidValueException>(
                  ::testing::AllOf(::testing::HasSubstr("64k"), ::testing::HasSubstr("--size"),
                                   ::testing::HasSubstr("is not an integer"))));

  EXPECT_THAT([] { (void)ConvertValue<unsigned int>("option \'--count\'", "-1"); },
              ::testing::ThrowsMessage<InvalidValueException>(
                  ::testing::AllOf(::testing::HasSubstr("-1"), ::testing::HasSubstr("--count"),
                                   ::testing::HasSubstr("non-negative"))));
}
