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

#include <sup/log/logger_t.h>

#include <tuple>
#include <vector>

#include <gtest/gtest.h>

using namespace sup::log;

const std::string LOG_SOURCE = "LoggerTTest";
const std::string MESSAGE_1 = "message 1";
const std::string MESSAGE_2 = "message 2";
const std::string MESSAGE_3 = "message 3";
const std::string MESSAGE_4 = "message 4";
const std::string MESSAGE_5 = "message 5";
const std::string MESSAGE_6 = "message 6";
const std::string MESSAGE_7 = "message 7";
const std::string MESSAGE_8 = "message 8";
const std::string MESSAGE_9 = "message 9";

class LoggerTTest : public ::testing::Test
{
protected:
  LoggerTTest();
  virtual ~LoggerTTest();

  using WarningFilterLogger = LoggerT<SUP_LOG_WARNING>;
  WarningFilterLogger CreateFilteredLogger(const std::string& source, int max_severity);

  using FilterNoneLogger = LoggerT<SUP_LOG_TRACE>;
  FilterNoneLogger CreateNoFilterLogger(const std::string& source);

  using LogEntry = std::tuple<int, std::string, std::string>;
  std::vector<LogEntry> m_log_entries;
};

TEST_F(LoggerTTest, FilterLogging)
{
  EXPECT_TRUE(m_log_entries.empty());

  // Create logger
  auto logger = CreateFilteredLogger(LOG_SOURCE, SUP_LOG_INFO);

  // Send some log messages
  EXPECT_NO_THROW(logger.Emergency(MESSAGE_1));
  EXPECT_EQ(m_log_entries.size(), 1);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_EMERG, LOG_SOURCE, MESSAGE_1));

  EXPECT_NO_THROW(logger.Alert(MESSAGE_2));
  EXPECT_EQ(m_log_entries.size(), 2);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_ALERT, LOG_SOURCE, MESSAGE_2));

  EXPECT_NO_THROW(logger.Critical(MESSAGE_3));
  EXPECT_EQ(m_log_entries.size(), 3);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_CRIT, LOG_SOURCE, MESSAGE_3));

  // Change source string and send more log messages
  const std::string new_source = "New Source";
  EXPECT_EQ(logger.SetSource(new_source), LOG_SOURCE);

  EXPECT_NO_THROW(logger.Error(MESSAGE_4));
  EXPECT_EQ(m_log_entries.size(), 4);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_ERR, new_source, MESSAGE_4));

  EXPECT_NO_THROW(logger.Warning(MESSAGE_5));
  EXPECT_EQ(m_log_entries.size(), 5);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_WARNING, new_source, MESSAGE_5));

  // Reset source string and log messages that will be filtered out
  EXPECT_EQ(logger.SetSource(LOG_SOURCE), new_source);

  EXPECT_NO_THROW(logger.Notice(MESSAGE_6));
  EXPECT_EQ(m_log_entries.size(), 5);
  EXPECT_NO_THROW(logger.Info(MESSAGE_6));
  EXPECT_EQ(m_log_entries.size(), 5);
  EXPECT_NO_THROW(logger.Debug(MESSAGE_6));
  EXPECT_EQ(m_log_entries.size(), 5);
  EXPECT_NO_THROW(logger.Trace(MESSAGE_6));
  EXPECT_EQ(m_log_entries.size(), 5);
}

TEST_F(LoggerTTest, SetMaxSeverity)
{
  EXPECT_TRUE(m_log_entries.empty());

  // Create logger
  auto logger = CreateFilteredLogger(LOG_SOURCE, SUP_LOG_ALERT);

  // Send some log messages
  EXPECT_NO_THROW(logger.Emergency(MESSAGE_1));
  EXPECT_EQ(m_log_entries.size(), 1);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_EMERG, LOG_SOURCE, MESSAGE_1));

  EXPECT_NO_THROW(logger.Alert(MESSAGE_2));
  EXPECT_EQ(m_log_entries.size(), 2);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_ALERT, LOG_SOURCE, MESSAGE_2));

  EXPECT_NO_THROW(logger.Critical(MESSAGE_3));
  EXPECT_EQ(m_log_entries.size(), 2);
  EXPECT_NO_THROW(logger.Error(MESSAGE_3));
  EXPECT_EQ(m_log_entries.size(), 2);
  EXPECT_NO_THROW(logger.Warning(MESSAGE_3));
  EXPECT_EQ(m_log_entries.size(), 2);
  EXPECT_NO_THROW(logger.Notice(MESSAGE_3));
  EXPECT_EQ(m_log_entries.size(), 2);
  EXPECT_NO_THROW(logger.Info(MESSAGE_3));
  EXPECT_EQ(m_log_entries.size(), 2);
  EXPECT_NO_THROW(logger.Debug(MESSAGE_3));
  EXPECT_EQ(m_log_entries.size(), 2);
  EXPECT_NO_THROW(logger.Trace(MESSAGE_3));
  EXPECT_EQ(m_log_entries.size(), 2);

  // Change runtime filter level and send log messages again
  EXPECT_EQ(logger.SetMaxSeverity(SUP_LOG_TRACE), SUP_LOG_ALERT);

  EXPECT_NO_THROW(logger.Critical(MESSAGE_3));
  EXPECT_EQ(m_log_entries.size(), 3);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_CRIT, LOG_SOURCE, MESSAGE_3));
  EXPECT_NO_THROW(logger.Error(MESSAGE_4));
  EXPECT_EQ(m_log_entries.size(), 4);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_ERR, LOG_SOURCE, MESSAGE_4));
  EXPECT_NO_THROW(logger.Warning(MESSAGE_5));
  EXPECT_EQ(m_log_entries.size(), 5);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_WARNING, LOG_SOURCE, MESSAGE_5));
  EXPECT_NO_THROW(logger.Notice(MESSAGE_6));
  EXPECT_EQ(m_log_entries.size(), 5);
  EXPECT_NO_THROW(logger.Info(MESSAGE_6));
  EXPECT_EQ(m_log_entries.size(), 5);
  EXPECT_NO_THROW(logger.Debug(MESSAGE_6));
  EXPECT_EQ(m_log_entries.size(), 5);
  EXPECT_NO_THROW(logger.Trace(MESSAGE_6));
  EXPECT_EQ(m_log_entries.size(), 5);
}

TEST_F(LoggerTTest, FilterNone)
{
  EXPECT_TRUE(m_log_entries.empty());

  // Create logger
  auto logger = CreateNoFilterLogger(LOG_SOURCE);

  // Send log messages
  EXPECT_NO_THROW(logger.Emergency(MESSAGE_1));
  EXPECT_EQ(m_log_entries.size(), 1);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_EMERG, LOG_SOURCE, MESSAGE_1));

  EXPECT_NO_THROW(logger.Alert(MESSAGE_2));
  EXPECT_EQ(m_log_entries.size(), 2);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_ALERT, LOG_SOURCE, MESSAGE_2));

  EXPECT_NO_THROW(logger.Critical(MESSAGE_3));
  EXPECT_EQ(m_log_entries.size(), 3);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_CRIT, LOG_SOURCE, MESSAGE_3));

  EXPECT_NO_THROW(logger.Error(MESSAGE_4));
  EXPECT_EQ(m_log_entries.size(), 4);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_ERR, LOG_SOURCE, MESSAGE_4));

  EXPECT_NO_THROW(logger.Warning(MESSAGE_5));
  EXPECT_EQ(m_log_entries.size(), 5);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_WARNING, LOG_SOURCE, MESSAGE_5));

  EXPECT_NO_THROW(logger.Notice(MESSAGE_6));
  EXPECT_EQ(m_log_entries.size(), 6);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_NOTICE, LOG_SOURCE, MESSAGE_6));

  EXPECT_NO_THROW(logger.Info(MESSAGE_7));
  EXPECT_EQ(m_log_entries.size(), 7);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_INFO, LOG_SOURCE, MESSAGE_7));

  EXPECT_NO_THROW(logger.Debug(MESSAGE_8));
  EXPECT_EQ(m_log_entries.size(), 8);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_DEBUG, LOG_SOURCE, MESSAGE_8));

  EXPECT_NO_THROW(logger.Trace(MESSAGE_9));
  EXPECT_EQ(m_log_entries.size(), 9);
  EXPECT_EQ(m_log_entries.back(), LogEntry(SUP_LOG_TRACE, LOG_SOURCE, MESSAGE_9));
}

LoggerTTest::LoggerTTest()
  : m_log_entries{}
{}

LoggerTTest::~LoggerTTest() = default;

LoggerTTest::WarningFilterLogger
LoggerTTest::CreateFilteredLogger(const std::string& source, int max_severity)
{
  auto log_func = [this](int severity, const std::string& source, const std::string& message)
                  {
                    m_log_entries.emplace_back(severity, source, message);
                  };
  return WarningFilterLogger(log_func, source, max_severity);
}

LoggerTTest::FilterNoneLogger
LoggerTTest::CreateNoFilterLogger(const std::string& source)
{
  auto log_func = [this](int severity, const std::string& source, const std::string& message)
                  {
                    m_log_entries.emplace_back(severity, source, message);
                  };
  return FilterNoneLogger(log_func, source, SUP_LOG_TRACE);
}
