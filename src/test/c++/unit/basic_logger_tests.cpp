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

#include <sup/log/basic_logger.h>

#include <gtest/gtest.h>

using namespace sup::log;

class BasicLoggerTest : public ::testing::Test
{
protected:
  BasicLoggerTest();
  virtual ~BasicLoggerTest();
};

TEST_F(BasicLoggerTest, DefaultConstructed)
{
  EXPECT_FALSE(false);
}

BasicLoggerTest::BasicLoggerTest()= default;

BasicLoggerTest::~BasicLoggerTest() = default;
