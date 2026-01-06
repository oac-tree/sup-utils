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

#include <sup/xml/exceptions.h>

#include <gtest/gtest.h>

using namespace sup::xml;

const std::string MESSAGE_1 = "message 1";
const std::string MESSAGE_2 = "message 2";
const std::string MESSAGE_3 = "message 3";

class XMLExceptionTest : public ::testing::Test
{
protected:
  XMLExceptionTest();
  virtual ~XMLExceptionTest();
};

TEST_F(XMLExceptionTest, InvalidOperation)
{
  InvalidOperationException exception{MESSAGE_1};
  EXPECT_EQ(exception.what(), MESSAGE_1);
}

TEST_F(XMLExceptionTest, Parse)
{
  ParseException exception{MESSAGE_2};
  EXPECT_EQ(exception.what(), MESSAGE_2);
}

TEST_F(XMLExceptionTest, Serialize)
{
  SerializeException exception{MESSAGE_3};
  EXPECT_EQ(exception.what(), MESSAGE_3);
}

TEST_F(XMLExceptionTest, NullptrCheck)
{
  int a;
  int* pa = &a;
  auto checked_ptr = AssertNoNullptr(pa, InvalidOperationException("doesnt_matter"));
  EXPECT_EQ(checked_ptr, pa);
  pa = nullptr;
  EXPECT_THROW(AssertNoNullptr(pa, InvalidOperationException("test")),
               InvalidOperationException);
}

XMLExceptionTest::XMLExceptionTest() = default;

XMLExceptionTest::~XMLExceptionTest() = default;
