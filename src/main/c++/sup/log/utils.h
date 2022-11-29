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

#ifndef SUP_LOG_UTILS_H_
#define SUP_LOG_UTILS_H_

#include <stdio.h>
#include <string>

namespace sup
{
namespace log
{

void SysLog(int severity, const std::string& message);

void StdoutLog(const std::string& message);

constexpr size_t kBufferSize = 1024;

template <typename... Args>
std::string FormatMessage(const std::string& format, Args&&... args)
{
  char buffer[kBufferSize];
  (void)snprintf(buffer, kBufferSize, format.c_str(), std::forward<Args>(args)...);
  return std::string(buffer);
}

}  // namespace log

}  // namespace sup

#endif  // SUP_LOG_UTILS_H_
