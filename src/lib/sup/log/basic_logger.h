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
 * Copyright (c) : 2010-2023 ITER Organization,
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

#include <sup/log/base_types.h>

#include <string>
#include <functional>

namespace sup
{
namespace log
{
/**
 * @brief BasicLogger encapsulates a basic logging function, a source string and a maximum severity
 * to log. It uses this maximum severity to discard logging for calls with higher severity,
 * i.e. less severe.
 *
 * @details This class does not use any semaphores to ensure thread-safety. Thread-safety depends
 * on the thread-safety of the logging function passed in the constructor. Note also that the
 * non-const member functions should never be called concurrently with any other member function.
 */
class BasicLogger
{
public:
  /**
   * @brief Constructor.
   *
   * @param log_func Function to call for each logging member function.
   * @param source Source identifier (will be passed to the logging function).
   * @param max_severity Maximum severity to log (used during runtime filtering).
   */
  BasicLogger(std::function<void(int32, const std::string&, const std::string&)> log_func,
              const std::string& source, int32 max_severity);
  /**
   * @brief Destructor.
   */
  ~BasicLogger();

  // Copy/move
  BasicLogger(const BasicLogger&) = default;
  BasicLogger& operator=(const BasicLogger&) = default;
  BasicLogger(BasicLogger&&) = default;
  BasicLogger& operator=(BasicLogger&&) = default;

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
   * @brief Log a message with the given severity.
   *
   * @param severity Severity level of the log message.
   * @param message Log message.
   *
   * @note The log message may be discarded when its severity level is higher than the current
   * maximum severity level.
   */
  void LogMessage(int32 severity, const std::string& message) const;

private:
  std::function<void(int32, const std::string&, const std::string&)> m_log_function;
  std::string m_source;
  int32 m_max_severity;
};

}  // namespace log

}  // namespace sup

#endif  // SUP_LOG_BASIC_LOGGER_H_
