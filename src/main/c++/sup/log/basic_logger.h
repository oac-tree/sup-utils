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

#ifndef SUP_LOG_BASIC_LOGGER_H_
#define SUP_LOG_BASIC_LOGGER_H_

#include <functional>
#include <stdio.h>
#include <string>

namespace sup
{
namespace log
{
/**
 * @brief BasicLogger encapsulates a basic logging function, a source string and a maximum severity
 * to log. It uses this maximum severity to discard logging for calls with higher severity,
 * i.e. less severe.
 */
class BasicLogger
{
public:
  BasicLogger(std::function<void(int, const std::string&, const std::string&)> log_func,
              const std::string& source, int max_severity);
  ~BasicLogger();

  int SetMaxSeverity(int max_severity);

  std::string SetSource(const std::string& source);

  void LogMessage(int severity, const std::string& message) const;

private:
  std::function<void(int, const std::string&, const std::string&)> m_log_function;
  std::string m_source;
  int m_max_severity;
};

}  // namespace log

}  // namespace sup

#endif  // SUP_LOG_BASIC_LOGGER_H_
