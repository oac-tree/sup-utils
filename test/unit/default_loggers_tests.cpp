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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/log/default_loggers.h>
#include <sup/log/log_severity.h>

#include <gtest/gtest.h>

using namespace sup::log;

const std::string LOG_SOURCE = "DefaultLoggersTest";
const std::string MESSAGE_1 = "message 1";
const std::string MESSAGE_2 = "message 2";
const std::string MESSAGE_3 = "message 3";
const std::string MESSAGE_4 = "message 4";
const std::string MESSAGE_5 = "message 5";
const std::string MESSAGE_6 = "message 6";
const std::string MESSAGE_7 = "message 7";
const std::string MESSAGE_8 = "message 8";
const std::string MESSAGE_9 = "message 9";

class DefaultLoggersTest : public ::testing::Test
{
protected:
  DefaultLoggersTest();
  virtual ~DefaultLoggersTest();
};

TEST_F(DefaultLoggersTest, StdoutLogger)
{
  // Create default stdout logger
  auto stdout_logger = CreateDefaultStdoutLogger(LOG_SOURCE);

  // log messages and ensure this doesn't throw
  EXPECT_NO_THROW(stdout_logger.Emergency(MESSAGE_1));
  EXPECT_NO_THROW(stdout_logger.Alert(MESSAGE_2));
  EXPECT_NO_THROW(stdout_logger.Critical(MESSAGE_3));

  // Change source and log messages
  std::string new_source = "New Source";
  EXPECT_EQ(stdout_logger.SetSource(new_source), LOG_SOURCE);

  EXPECT_NO_THROW(stdout_logger.Error(MESSAGE_4));
  EXPECT_NO_THROW(stdout_logger.Warning(MESSAGE_5));
  EXPECT_NO_THROW(stdout_logger.Notice(MESSAGE_6));
  EXPECT_NO_THROW(stdout_logger.Info(MESSAGE_7));
  EXPECT_NO_THROW(stdout_logger.Debug(MESSAGE_8));
  EXPECT_NO_THROW(stdout_logger.Trace(MESSAGE_9));

  // Change max severity, reset source and log messages
  EXPECT_EQ(stdout_logger.SetMaxSeverity(SUP_LOG_ALERT), kDefaultMaxEnabledSeverity);
  EXPECT_EQ(stdout_logger.SetSource(LOG_SOURCE), new_source);

  EXPECT_NO_THROW(stdout_logger.Emergency(MESSAGE_1));
  EXPECT_NO_THROW(stdout_logger.Alert(MESSAGE_2));
  EXPECT_NO_THROW(stdout_logger.Critical(MESSAGE_3));
}

TEST_F(DefaultLoggersTest, SysLogger)
{
  // Create default stdout logger
  auto stdout_logger = CreateDefaultSysLogger(LOG_SOURCE);

  // log messages and ensure this doesn't throw
  // EXPECT_NO_THROW(stdout_logger.Emergency(MESSAGE_1));
  EXPECT_NO_THROW(stdout_logger.Alert(MESSAGE_2));
  EXPECT_NO_THROW(stdout_logger.Critical(MESSAGE_3));

  // Change source and log messages
  std::string new_source = "New Source";
  EXPECT_EQ(stdout_logger.SetSource(new_source), LOG_SOURCE);

  EXPECT_NO_THROW(stdout_logger.Error(MESSAGE_4));
  EXPECT_NO_THROW(stdout_logger.Warning(MESSAGE_5));
  EXPECT_NO_THROW(stdout_logger.Notice(MESSAGE_6));
  EXPECT_NO_THROW(stdout_logger.Info(MESSAGE_7));
  EXPECT_NO_THROW(stdout_logger.Debug(MESSAGE_8));
  EXPECT_NO_THROW(stdout_logger.Trace(MESSAGE_9));

  // Change max severity, reset source and log messages
  EXPECT_EQ(stdout_logger.SetMaxSeverity(SUP_LOG_ALERT), kDefaultMaxEnabledSeverity);
  EXPECT_EQ(stdout_logger.SetSource(LOG_SOURCE), new_source);

  // EXPECT_NO_THROW(stdout_logger.Emergency(MESSAGE_1));
  EXPECT_NO_THROW(stdout_logger.Alert(MESSAGE_2));
  EXPECT_NO_THROW(stdout_logger.Critical(MESSAGE_3));
}

DefaultLoggersTest::DefaultLoggersTest() = default;

DefaultLoggersTest::~DefaultLoggersTest() = default;
