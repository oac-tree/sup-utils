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

#ifndef SUP_LOG_LOGGER_T_H_
#define SUP_LOG_LOGGER_T_H_

#include "log_severity.h"
#include "basic_logger.h"

#include <functional>
#include <string>

namespace sup
{
namespace log
{

/**
 * @brief LoggerT encapsulates a BasicLogger and disables at compile time all log messages
 * with a severity higher than max_enabled (i.e. less severe).
 */
template <int max_enabled>
class LoggerT
{
public:
  LoggerT(std::function<void(int, const std::string&, const std::string&)> log_func,
          const std::string& source, int max_severity = max_enabled);
  ~LoggerT();

  int SetMaxSeverity(int max_severity);

  std::string SetSource(const std::string& source);

  void Emergency(const std::string& message) const;
  void Alert(const std::string& message) const;
  void Critical(const std::string& message) const;
  void Error(const std::string& message) const;
  void Warning(const std::string& message) const;
  void Notice(const std::string& message) const;
  void Info(const std::string& message) const;
  void Debug(const std::string& message) const;
  void Trace(const std::string& message) const;

private:
  template <bool b, typename std::enable_if<b, bool>::type = true>
  void ConditionalLog(int severity, const std::string& message) const
  {
    m_logger_impl.LogMessage(severity, message);
  }

  template <bool b, typename std::enable_if<!b, bool>::type = true>
  void ConditionalLog(int, const std::string&) const
  {}

  BasicLogger m_logger_impl;
};

template <int max_enabled>
LoggerT<max_enabled>::LoggerT(
  std::function<void(int, const std::string&, const std::string&)> log_func,
  const std::string& source, int max_severity)
  : m_logger_impl(log_func, source, max_severity)
{}

template <int max_enabled>
LoggerT<max_enabled>::~LoggerT() = default;

template <int max_enabled>
int LoggerT<max_enabled>::SetMaxSeverity(int max_severity)
{
  return m_logger_impl.SetMaxSeverity(max_severity);
}

template <int max_enabled>
std::string LoggerT<max_enabled>::SetSource(const std::string& source)
{
  return m_logger_impl.SetSource(source);
}

template <int max_enabled>
void LoggerT<max_enabled>::Emergency(const std::string& message) const
{
  ConditionalLog<(max_enabled >= SUP_LOG_EMERG)>(SUP_LOG_EMERG, message);
}

template <int max_enabled>
void LoggerT<max_enabled>::Alert(const std::string& message) const
{
  ConditionalLog<(max_enabled >= SUP_LOG_ALERT)>(SUP_LOG_ALERT, message);
}

template <int max_enabled>
void LoggerT<max_enabled>::Critical(const std::string& message) const
{
  ConditionalLog<(max_enabled >= SUP_LOG_CRIT)>(SUP_LOG_CRIT, message);
}

template <int max_enabled>
void LoggerT<max_enabled>::Error(const std::string& message) const
{
  ConditionalLog<(max_enabled >= SUP_LOG_ERR)>(SUP_LOG_ERR, message);
}

template <int max_enabled>
void LoggerT<max_enabled>::Warning(const std::string& message) const
{
  ConditionalLog<(max_enabled >= SUP_LOG_WARNING)>(SUP_LOG_WARNING, message);
}

template <int max_enabled>
void LoggerT<max_enabled>::Notice(const std::string& message) const
{
  ConditionalLog<(max_enabled >= SUP_LOG_NOTICE)>(SUP_LOG_NOTICE, message);
}

template <int max_enabled>
void LoggerT<max_enabled>::Info(const std::string& message) const
{
  ConditionalLog<(max_enabled >= SUP_LOG_INFO)>(SUP_LOG_INFO, message);
}

template <int max_enabled>
void LoggerT<max_enabled>::Debug(const std::string& message) const
{
  ConditionalLog<(max_enabled >= SUP_LOG_DEBUG)>(SUP_LOG_DEBUG, message);
}

template <int max_enabled>
void LoggerT<max_enabled>::Trace(const std::string& message) const
{
  ConditionalLog<(max_enabled >= SUP_LOG_TRACE)>(SUP_LOG_TRACE, message);
}

}  // namespace log

}  // namespace sup

#endif  // SUP_LOG_LOGGER_T_H_
