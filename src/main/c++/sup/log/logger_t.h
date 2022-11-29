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

#include <functional>
#include <string>

namespace sup
{
namespace log
{

/**
 * @brief LoggerT encapsulates a logger implementation and disables at compile all log messages
 * with a severity of disable_level and lower.
 */
template <typename TLog, int disable_level>
class LoggerT
{
public:
  LoggerT(std::function<void(int, const std::string&, const std::string&)> log_func,
          const std::string& source, int max_severity);
  ~LoggerT();

  int SetMaxSeverity(int max_severity);

  std::string SetSource(const std::string& source);

  void Emergency(const std::string& message) const;

private:
  template <bool b, typename std::enable_if<b, bool>::type = true>
  void ConditionalLog(int severity, const std::string& message) const
  {
    m_logger_impl.LogMessage(severity, message);
  }

  template <bool b, typename std::enable_if<!b, bool>::type = true>
  void ConditionalLog(int, const std::string&) const
  {}

  TLog m_logger_impl;
};

template <typename TLog, int disable_level>
void LoggerT<TLog, disable_level>::Emergency(const std::string& message) const
{
  ConditionalLog<(disable_level > SUP_LOG_EMERG)>(SUP_LOG_EMERG, message);
}

}  // namespace log

}  // namespace sup

#endif  // SUP_LOG_LOGGER_T_H_
