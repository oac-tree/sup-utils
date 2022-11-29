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

#include "log_severity.h"

#include <map>

namespace
{
const std::map<int, std::string>& GetSeverityMap();
}

namespace sup
{
namespace log
{
const std::string EmergencyString = "EMERGENCY";
const std::string AlertString = "ALERT";
const std::string CriticalString = "CRITICAL";
const std::string ErrorString = "ERROR";
const std::string WarningString = "WARNING";
const std::string NoticeString = "NOTICE";
const std::string InfoString = "INFO";
const std::string DebugString = "DEBUG";
const std::string TraceString = "TRACE";

std::string SeverityString(int severity)
{
  const auto& sev_map = GetSeverityMap();
  auto it = sev_map.find(severity);
  if (it == sev_map.end())
  {
    return "UNKNOWN";
  }
  return it->second;
}

}  // namespace log

}  // namespace sup

namespace
{
const std::map<int, std::string>& GetSeverityMap()
{
  static std::map<int, std::string> severity_map = {
    { sup::log::SUP_LOG_EMERG, sup::log::EmergencyString},
    { sup::log::SUP_LOG_ALERT, sup::log::AlertString},
    { sup::log::SUP_LOG_CRIT, sup::log::CriticalString},
    { sup::log::SUP_LOG_ERR, sup::log::ErrorString},
    { sup::log::SUP_LOG_WARNING, sup::log::WarningString},
    { sup::log::SUP_LOG_NOTICE, sup::log::NoticeString},
    { sup::log::SUP_LOG_INFO, sup::log::InfoString},
    { sup::log::SUP_LOG_DEBUG, sup::log::DebugString},
    { sup::log::SUP_LOG_TRACE, sup::log::TraceString}
  };
  return severity_map;
}
}
