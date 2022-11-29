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

#include "default_loggers.h"

#include "utils.h"

#include <sstream>

#include <unistd.h>

namespace sup
{
namespace log
{

std::string DefaultLogMessage(int severity, const std::string& source, const std::string& message)
{
  std::ostringstream oss;
  oss << "sup-log-lib:" << getpid() << "]";
  oss << "[" << source << "]";
  oss << "[" << SeverityString(severity) << "] ";
  oss << message;
  return oss.str();
}

DefaultLogger CreateDefaultStdoutLogger(const std::string& source)
{
  return DefaultLogger([](int severity, const std::string& source, const std::string& message){
                         StdoutLog(DefaultLogMessage(severity, source, message));
                       }, source, SUP_LOG_INFO);
}

DefaultLogger CreateDefaultSysLogger(const std::string& source)
{
  return DefaultLogger([](int severity, const std::string& source, const std::string& message){
                         SysLog(severity, DefaultLogMessage(severity, source, message));
                       }, source, SUP_LOG_INFO);
}

}  // namespace log

}  // namespace sup
