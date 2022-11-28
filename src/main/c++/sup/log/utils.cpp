/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
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

#include "utils.h"

#include <iostream>
#include <sstream>

#include <syslog.h>
#include <unistd.h>

namespace sup
{
namespace log
{
std::string SeverityString(int severity);

std::string LogMessage(int severity, const std::string& source, const std::string& message)
{
  std::ostringstream oss;
  oss << "sup-log-lib:" << getpid() << "]";
  oss << "[" << source << "]";
  oss << "[" << SeverityString(severity) << "] ";
  oss << message;
  return oss.str();
}

void SysLog(int severity, const std::string& source, const std::string& message)
{
  syslog(severity, LogMessage(severity, source, message).c_str());
}

void StdoutLog(int severity, const std::string& source, const std::string& message)
{
  std::cout << LogMessage(severity, source, message) << std::endl;
}

}  // namespace log

}  // namespace sup
