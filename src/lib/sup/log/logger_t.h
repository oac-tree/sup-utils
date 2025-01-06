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

#ifndef SUP_LOG_LOGGER_T_H_
#define SUP_LOG_LOGGER_T_H_

#include "log_severity.h"
#include "basic_logger.h"
#include "base_types.h"

#include <functional>
#include <string>

namespace sup
{
namespace log
{

/**
 * @brief LoggerT encapsulates a BasicLogger and disables at compile time all log messages
 * with a severity higher than max_enabled (i.e. less severe).
 *
 * @details This class template also supports runtime log filtering through its constructor
 * parameter 'max_severity'. The runtime filter level can be changed by calling the member function
 * 'SetMaxSeverity'.
 *
 * @details This class does not use any semaphores to ensure thread-safety. Thread-safety depends
 * on the thread-safety of the logging function passed in the constructor. Note also that the
 * non-const member functions should never be called concurrently with any other member function.
 */
template <int32 max_enabled>
class LoggerT
{
public:
  /**
   * @brief Constructor.
   *
   * @param log_func Function to call for each logging member function.
   * @param source Source identifier (will be passed to the logging function).
   * @param max_severity Maximum severity to log (used during runtime filtering).
   */
  LoggerT(std::function<void(int32, const std::string&, const std::string&)> log_func,
          const std::string& source, int32 max_severity = max_enabled);
  /**
   * @brief Destructor.
   */
  ~LoggerT();

  /**
   * @brief Change the maximum severity for filtering.
   *
   * @param max_severity Maximum severity to log (used during runtime filtering).
   *
   * @return Previous maximum severity.
   */
  int32 SetMaxSeverity(int32 max_severity);

  /**
   * @brief Change the source identifier for logging.
   *
   * @param source New source identifier to use for logging.
   *
   * @return Previous source identifier.
   */
  std::string SetSource(const std::string& source);

  /**
   * @brief Log a message with the severity: EMERGENCY.
   *
   * @param message Log message.
   *
   * @note The log message may be discarded when its severity level is higher than the current
   * maximum severity level.
   */
  void Emergency(const std::string& message) const;
  /**
   * @brief Log a message with the severity: ALERT.
   *
   * @param message Log message.
   *
   * @note The log message may be discarded when its severity level is higher than the current
   * maximum severity level.
   */
  void Alert(const std::string& message) const;
  /**
   * @brief Log a message with the severity: CRITICAL.
   *
   * @param message Log message.
   *
   * @note The log message may be discarded when its severity level is higher than the current
   * maximum severity level.
   */
  void Critical(const std::string& message) const;
  /**
   * @brief Log a message with the severity: ERROR.
   *
   * @param message Log message.
   *
   * @note The log message may be discarded when its severity level is higher than the current
   * maximum severity level.
   */
  void Error(const std::string& message) const;
  /**
   * @brief Log a message with the severity: WARNING.
   *
   * @param message Log message.
   *
   * @note The log message may be discarded when its severity level is higher than the current
   * maximum severity level.
   */
  void Warning(const std::string& message) const;
  /**
   * @brief Log a message with the severity: NOTICE.
   *
   * @param message Log message.
   *
   * @note The log message may be discarded when its severity level is higher than the current
   * maximum severity level.
   */
  void Notice(const std::string& message) const;
  /**
   * @brief Log a message with the severity: INFO.
   *
   * @param message Log message.
   *
   * @note The log message may be discarded when its severity level is higher than the current
   * maximum severity level.
   */
  void Info(const std::string& message) const;
  /**
   * @brief Log a message with the severity: DEBUG.
   *
   * @param message Log message.
   *
   * @note The log message may be discarded when its severity level is higher than the current
   * maximum severity level.
   */
  void Debug(const std::string& message) const;
  /**
   * @brief Log a message with the severity: TRACE.
   *
   * @param message Log message.
   *
   * @note The log message may be discarded when its severity level is higher than the current
   * maximum severity level.
   */
  void Trace(const std::string& message) const;

private:
  template <bool b, typename std::enable_if<b, bool>::type = true>
  void ConditionalLog(int32 severity, const std::string& message) const
  {
    m_logger_impl.LogMessage(severity, message);
  }

  template <bool b, typename std::enable_if<!b, bool>::type = true>
  void ConditionalLog(int32, const std::string&) const
  {}

  BasicLogger m_logger_impl;
};

template <int32 max_enabled>
LoggerT<max_enabled>::LoggerT(
  std::function<void(int32, const std::string&, const std::string&)> log_func,
  const std::string& source, int32 max_severity)
  : m_logger_impl(log_func, source, max_severity)
{}

template <int32 max_enabled>
LoggerT<max_enabled>::~LoggerT() = default;

template <int32 max_enabled>
int32 LoggerT<max_enabled>::SetMaxSeverity(int32 max_severity)
{
  return m_logger_impl.SetMaxSeverity(max_severity);
}

template <int32 max_enabled>
std::string LoggerT<max_enabled>::SetSource(const std::string& source)
{
  return m_logger_impl.SetSource(source);
}

template <int32 max_enabled>
void LoggerT<max_enabled>::Emergency(const std::string& message) const
{
  ConditionalLog<(max_enabled >= SUP_LOG_EMERG)>(SUP_LOG_EMERG, message);
}

template <int32 max_enabled>
void LoggerT<max_enabled>::Alert(const std::string& message) const
{
  ConditionalLog<(max_enabled >= SUP_LOG_ALERT)>(SUP_LOG_ALERT, message);
}

template <int32 max_enabled>
void LoggerT<max_enabled>::Critical(const std::string& message) const
{
  ConditionalLog<(max_enabled >= SUP_LOG_CRIT)>(SUP_LOG_CRIT, message);
}

template <int32 max_enabled>
void LoggerT<max_enabled>::Error(const std::string& message) const
{
  ConditionalLog<(max_enabled >= SUP_LOG_ERR)>(SUP_LOG_ERR, message);
}

template <int32 max_enabled>
void LoggerT<max_enabled>::Warning(const std::string& message) const
{
  ConditionalLog<(max_enabled >= SUP_LOG_WARNING)>(SUP_LOG_WARNING, message);
}

template <int32 max_enabled>
void LoggerT<max_enabled>::Notice(const std::string& message) const
{
  ConditionalLog<(max_enabled >= SUP_LOG_NOTICE)>(SUP_LOG_NOTICE, message);
}

template <int32 max_enabled>
void LoggerT<max_enabled>::Info(const std::string& message) const
{
  ConditionalLog<(max_enabled >= SUP_LOG_INFO)>(SUP_LOG_INFO, message);
}

template <int32 max_enabled>
void LoggerT<max_enabled>::Debug(const std::string& message) const
{
  ConditionalLog<(max_enabled >= SUP_LOG_DEBUG)>(SUP_LOG_DEBUG, message);
}

template <int32 max_enabled>
void LoggerT<max_enabled>::Trace(const std::string& message) const
{
  ConditionalLog<(max_enabled >= SUP_LOG_TRACE)>(SUP_LOG_TRACE, message);
}

}  // namespace log

}  // namespace sup

#endif  // SUP_LOG_LOGGER_T_H_
