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

#ifndef SUP_LOG_DEFAULT_LOGGERS_H_
#define SUP_LOG_DEFAULT_LOGGERS_H_

#include "basic_logger.h"
#include "logger_t.h"

namespace sup
{
namespace log
{
constexpr int kDefaultMaxEnabledSeverity = SUP_LOG_INFO;

using DefaultLogger = LoggerT<kDefaultMaxEnabledSeverity>;

std::string DefaultLogMessage(int severity, const std::string& source, const std::string& message);

DefaultLogger CreateDefaultStdoutLogger(const std::string& source);
DefaultLogger CreateDefaultSysLogger(const std::string& source);

}  // namespace log

}  // namespace sup

#endif  // SUP_LOG_DEFAULT_LOGGERS_H_
