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
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/log/log_severity.h>

#include <gtest/gtest.h>

using namespace sup::log;

const std::string LOG_SOURCE = "LogSeverityTest";
const std::string MESSAGE_1 = "message 1";
const std::string MESSAGE_2 = "message 2";
const std::string MESSAGE_3 = "message 3";
const std::string MESSAGE_4 = "message 4";
const std::string MESSAGE_5 = "message 5";
const std::string MESSAGE_6 = "message 6";
const std::string MESSAGE_7 = "message 7";

class LogSeverityTest : public ::testing::Test
{
protected:
  LogSeverityTest();
  virtual ~LogSeverityTest();
};

TEST_F(LogSeverityTest, SeverityString)
{
  // Standard severity level strings
  EXPECT_EQ(SeverityString(SUP_LOG_EMERG), "EMERGENCY");
  EXPECT_EQ(SeverityString(SUP_LOG_ALERT), "ALERT");
  EXPECT_EQ(SeverityString(SUP_LOG_CRIT), "CRITICAL");
  EXPECT_EQ(SeverityString(SUP_LOG_ERR), "ERROR");
  EXPECT_EQ(SeverityString(SUP_LOG_WARNING), "WARNING");
  EXPECT_EQ(SeverityString(SUP_LOG_NOTICE), "NOTICE");
  EXPECT_EQ(SeverityString(SUP_LOG_INFO), "INFO");
  EXPECT_EQ(SeverityString(SUP_LOG_DEBUG), "DEBUG");
  EXPECT_EQ(SeverityString(SUP_LOG_TRACE), "TRACE");

  // Unknown severity level strings
  EXPECT_EQ(SeverityString(NUMBER_OF_LOG_LEVELS), "UNKNOWN");
  EXPECT_EQ(SeverityString(-1), "UNKNOWN");
  EXPECT_EQ(SeverityString(-709876), "UNKNOWN");
  EXPECT_EQ(SeverityString(NUMBER_OF_LOG_LEVELS), "UNKNOWN");
  EXPECT_EQ(SeverityString(13844), "UNKNOWN");
}

LogSeverityTest::LogSeverityTest() = default;

LogSeverityTest::~LogSeverityTest() = default;
