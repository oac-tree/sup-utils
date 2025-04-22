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

#include "utils.h"

#include <iostream>

#include <syslog.h>

namespace sup
{
namespace log
{

void SysLog(int32 severity, const std::string& message)
{
  syslog(severity, "%s", message.c_str());
}

void StdoutLog(const std::string& message)
{
  std::cout << message << std::endl;
}

}  // namespace log

}  // namespace sup
