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

#ifndef SUP_LOG_UTILS_H_
#define SUP_LOG_UTILS_H_

#include "base_types.h"

#include <string>

namespace sup
{
namespace log
{

/**
 * @brief Send the given log message to the system log with the given severity level.
 */
void SysLog(int32 severity, const std::string& message);

/**
 * @brief Print the given log message directly to std::out.
 */
void StdoutLog(const std::string& message);

}  // namespace log

}  // namespace sup

#endif  // SUP_LOG_UTILS_H_
