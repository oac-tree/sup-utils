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

#include <sup/log/basic_logger.h>
#include <sup/log/log_severity.h>

#include <tuple>
#include <vector>

#include <gtest/gtest.h>

using namespace sup::log;

const std::string LOG_SOURCE = "BasicLoggerTest";
const std::string MESSAGE_1 = "message 1";
const std::string MESSAGE_2 = "message 2";
const std::string MESSAGE_3 = "message 3";
const std::string MESSAGE_4 = "message 4";
const std::string MESSAGE_5 = "message 5";
const std::string MESSAGE_6 = "message 6";
const std::string MESSAGE_7 = "message 7";

class BasicLoggerTest : public ::testing::Test
{
protected:
  BasicLoggerTest();
  virtual ~BasicLoggerTest();

  BasicLogger CreateBasicLogger(const std::string& source, int max_severity);

  using LogEntry = std::tuple<int, std::string, std::string>;
  std::vector<LogEntry> m_log_entries;
};

TEST_F(BasicLoggerTest, SetSource)
{
  EXPECT_TRUE(m_log_entries.empty());

  // Create logger
  auto logger = CreateBasicLogger(LOG_SOURCE, SUP_LOG_NOTICE);

  // Send some log messages
  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_EMERG, MESSAGE_1));
  EXPECT_EQ(m_log_entries.size(), 1);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_EMERG, LOG_SOURCE, MESSAGE_1));

  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_ALERT, MESSAGE_2));
  EXPECT_EQ(m_log_entries.size(), 2);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_ALERT, LOG_SOURCE, MESSAGE_2));

  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_CRIT, MESSAGE_3));
  EXPECT_EQ(m_log_entries.size(), 3);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_CRIT, LOG_SOURCE, MESSAGE_3));

  // Change source string and send more log messages
  const std::string new_source = "New Source";
  EXPECT_EQ(logger.SetSource(new_source), LOG_SOURCE);

  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_ERR, MESSAGE_4));
  EXPECT_EQ(m_log_entries.size(), 4);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_ERR, new_source, MESSAGE_4));

  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_WARNING, MESSAGE_5));
  EXPECT_EQ(m_log_entries.size(), 5);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_WARNING, new_source, MESSAGE_5));

  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_NOTICE, MESSAGE_6));
  EXPECT_EQ(m_log_entries.size(), 6);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_NOTICE, new_source, MESSAGE_6));

  // Reset source string and log messages that will be filtered out
  EXPECT_EQ(logger.SetSource(LOG_SOURCE), new_source);

  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_INFO, MESSAGE_7));
  EXPECT_EQ(m_log_entries.size(), 6);
  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_DEBUG, MESSAGE_7));
  EXPECT_EQ(m_log_entries.size(), 6);
  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_TRACE, MESSAGE_7));
  EXPECT_EQ(m_log_entries.size(), 6);
}

TEST_F(BasicLoggerTest, SetMaxSeverity)
{
  EXPECT_TRUE(m_log_entries.empty());

  // Create logger
  auto logger = CreateBasicLogger(LOG_SOURCE, SUP_LOG_NOTICE);

  // Send some log messages
  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_NOTICE, MESSAGE_1));
  EXPECT_EQ(m_log_entries.size(), 1);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_NOTICE, LOG_SOURCE, MESSAGE_1));

  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_INFO, MESSAGE_2));
  EXPECT_EQ(m_log_entries.size(), 1);

  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_DEBUG, MESSAGE_3));
  EXPECT_EQ(m_log_entries.size(), 1);

  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_TRACE, MESSAGE_4));
  EXPECT_EQ(m_log_entries.size(), 1);

  // Change filter level
  EXPECT_EQ(logger.SetMaxSeverity(SUP_LOG_DEBUG), SUP_LOG_NOTICE);

  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_INFO, MESSAGE_2));
  EXPECT_EQ(m_log_entries.size(), 2);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_INFO, LOG_SOURCE, MESSAGE_2));

  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_DEBUG, MESSAGE_3));
  EXPECT_EQ(m_log_entries.size(), 3);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_DEBUG, LOG_SOURCE, MESSAGE_3));

  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_TRACE, MESSAGE_4));
  EXPECT_EQ(m_log_entries.size(), 3);

  // Change filter level to Emergency only
  EXPECT_EQ(logger.SetMaxSeverity(SUP_LOG_EMERG), SUP_LOG_DEBUG);

  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_EMERG, MESSAGE_4));
  EXPECT_EQ(m_log_entries.size(), 4);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_EMERG, LOG_SOURCE, MESSAGE_4));

  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_ALERT, MESSAGE_5));
  EXPECT_EQ(m_log_entries.size(), 4);

  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_CRIT, MESSAGE_5));
  EXPECT_EQ(m_log_entries.size(), 4);

  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_ERR, MESSAGE_5));
  EXPECT_EQ(m_log_entries.size(), 4);

  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_WARNING, MESSAGE_5));
  EXPECT_EQ(m_log_entries.size(), 4);

  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_NOTICE, MESSAGE_5));
  EXPECT_EQ(m_log_entries.size(), 4);

  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_INFO, MESSAGE_5));
  EXPECT_EQ(m_log_entries.size(), 4);

  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_DEBUG, MESSAGE_5));
  EXPECT_EQ(m_log_entries.size(), 4);

  EXPECT_NO_THROW(logger.LogMessage(SUP_LOG_TRACE, MESSAGE_5));
  EXPECT_EQ(m_log_entries.size(), 4);
}

BasicLoggerTest::BasicLoggerTest()
  : m_log_entries{}
{}

BasicLoggerTest::~BasicLoggerTest() = default;

BasicLogger BasicLoggerTest::CreateBasicLogger(const std::string& source, int max_severity)
{
  auto log_func = [this](int severity, const std::string& source, const std::string& message)
                  {
                    m_log_entries.emplace_back(severity, source, message);
                  };
  return BasicLogger(log_func, source, max_severity);
}
