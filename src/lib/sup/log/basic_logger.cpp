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

#include "basic_logger.h"

namespace sup
{
namespace log
{

  BasicLogger::BasicLogger(std::function<void(int32, const std::string&, const std::string&)> log_func,
                         const std::string& source, int32 max_severity)
  : m_log_function{log_func}
  , m_source{source}
  , m_max_severity{max_severity}
{}

BasicLogger::~BasicLogger() = default;

BasicLogger::BasicLogger(const BasicLogger& other) = default;

BasicLogger& BasicLogger::operator=(const BasicLogger& other) & = default;

BasicLogger::BasicLogger(BasicLogger&&) noexcept = default;

BasicLogger& BasicLogger::operator=(BasicLogger&& other) & noexcept = default;

int32 BasicLogger::SetMaxSeverity(int32 max_severity)
{
  auto current_max_severity = m_max_severity;
  m_max_severity = max_severity;
  return current_max_severity;
}

std::string BasicLogger::SetSource(const std::string& source)
{
  auto current_source = m_source;
  m_source = source;
  return current_source;
}

void BasicLogger::LogMessage(int32 severity, const std::string& message) const
{
  if (severity > m_max_severity)
  {
    return;
  }
  m_log_function(severity, m_source, message);
}

}  // namespace log

}  // namespace sup
