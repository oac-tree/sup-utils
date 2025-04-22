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

#ifndef SUP_LOG_LOG_SEVERITY_H_
#define SUP_LOG_LOG_SEVERITY_H_

#include "base_types.h"

#include <string>

namespace sup
{
namespace log
{
enum Severity : int32
{
  SUP_LOG_EMERG = 0,
  SUP_LOG_ALERT,
  SUP_LOG_CRIT,
  SUP_LOG_ERR,
  SUP_LOG_WARNING,
  SUP_LOG_NOTICE,
  SUP_LOG_INFO,
  SUP_LOG_DEBUG,
  SUP_LOG_TRACE,
  NUMBER_OF_LOG_LEVELS
};

/**
 * @brief Retrieve a string representation of the given severity level.
 */
std::string SeverityString(int32 severity);

}  // namespace log

}  // namespace sup

#endif  // SUP_LOG_LOG_SEVERITY_H_
