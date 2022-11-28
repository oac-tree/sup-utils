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

#ifndef SUP_UTILS_LOG_UTILS_H_
#define SUP_UTILS_LOG_UTILS_H_

#include <string>

namespace sup
{
namespace log
{
std::string SeverityString(int severity);

std::string LogMessage(int severity, const std::string& source, const std::string& message);

void SysLog(int severity, const std::string& source, const std::string& message);

void StdoutLog(int severity, const std::string& source, const std::string& message);

}  // namespace log

}  // namespace sup

#endif  // SUP_UTILS_LOG_UTILS_H_
