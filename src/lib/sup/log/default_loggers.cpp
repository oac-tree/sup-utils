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

#include "default_loggers.h"

#include "utils.h"

#include <sstream>

#include <unistd.h>

namespace sup
{
namespace log
{

std::string DefaultStdoutLogMessage(int32 severity, const std::string& source, const std::string& message)
{
  std::ostringstream oss;
  oss << "sup-log[" << getpid() << "]: ";
  oss << "[" << source << "]";
  oss << "[" << SeverityString(severity) << "] ";
  oss << message;
  return oss.str();
}

std::string DefaultSysLogMessage(int32 severity, const std::string& source, const std::string& message)
{
  std::ostringstream oss;
  oss << "sup-log[" << source << "]";
  oss << "[" << SeverityString(severity) << "] ";
  oss << message;
  return oss.str();
}

DefaultLogger CreateDefaultStdoutLogger(const std::string& source)
{
  return DefaultLogger([](int32 severity, const std::string& source, const std::string& message){
                         StdoutLog(DefaultStdoutLogMessage(severity, source, message));
                       }, source);
}

DefaultLogger CreateDefaultSysLogger(const std::string& source)
{
  return DefaultLogger([](int32 severity, const std::string& source, const std::string& message){
                         SysLog(severity, DefaultSysLogMessage(severity, source, message));
                       }, source);
}

}  // namespace log

}  // namespace sup
