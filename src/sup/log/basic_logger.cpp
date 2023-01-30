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
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "basic_logger.h"

namespace sup
{
namespace log
{

BasicLogger::BasicLogger(std::function<void(int, const std::string&, const std::string&)> log_func,
                         const std::string& source, int max_severity)
  : m_log_function{log_func}
  , m_source{source}
  , m_max_severity{max_severity}
{}

BasicLogger::~BasicLogger() = default;

int BasicLogger::SetMaxSeverity(int max_severity)
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

void BasicLogger::LogMessage(int severity, const std::string& message) const
{
  if (severity > m_max_severity)
  {
    return;
  }
  m_log_function(severity, m_source, message);
}

}  // namespace log

}  // namespace sup
