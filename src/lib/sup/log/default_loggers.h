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

#ifndef SUP_LOG_DEFAULT_LOGGERS_H_
#define SUP_LOG_DEFAULT_LOGGERS_H_

#include "basic_logger.h"
#include "logger_t.h"

namespace sup
{
namespace log
{
constexpr int32_t kDefaultMaxEnabledSeverity = SUP_LOG_INFO;

/**
 * @brief Define the DefaultLogger class as a logger that discards log messages with severity higher
 * than 'kDefaultMaxEnabledSeverity' at compile time.
 */
using DefaultLogger = LoggerT<kDefaultMaxEnabledSeverity>;

/**
 * @brief Create a default formatted log message from the given parameters, intended for output
 * to a regular string stream (e.g. std::out).
 */
std::string DefaultStdoutLogMessage(int32_t severity, const std::string& source, const std::string& message);
/**
 * @brief Create a default formatted log message from the given parameters, intended for output
 * to a system log.
 */
std::string DefaultSysLogMessage(int32_t severity, const std::string& source, const std::string& message);

/**
 * @brief Create a default logger with the given source identifier that outputs a default formatted
 * log message directly to std::out.
 */
DefaultLogger CreateDefaultStdoutLogger(const std::string& source);
/**
 * @brief Create a default logger with the given source identifier that outputs a default formatted
 * log message directly to the system log.
 */
DefaultLogger CreateDefaultSysLogger(const std::string& source);

}  // namespace log

}  // namespace sup

#endif  // SUP_LOG_DEFAULT_LOGGERS_H_
