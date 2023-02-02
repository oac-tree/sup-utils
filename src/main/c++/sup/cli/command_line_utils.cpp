/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : Supervision and Automation System Utilities
 *
 * Description   : Command line argument parsing
 *
 * Author        : Gennady Pospelov (IO)
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

#include "command_line_utils.h"

#include <sup/cli/command_line_option.h>

#include <numeric>

namespace
{
const int kDesiredOptionStringLength = 30;

/**
 * Returns value name.
 */
std::string GetValueName(const sup::cli::CommandLineOption &option)
{
  return option.GetValueName().empty() ? std::string("value") : option.GetValueName();
}

}  // namespace

namespace sup
{
namespace cli
{

std::string GetAvailableOptionsSummaryString(const std::vector<const CommandLineOption *> &options)
{
  (void)options;
  // currently returns simplified representation of all options
  return {"[options]"};
}

std::string GetOptionUsageString(const CommandLineOption &option)
{
  const auto names = option.GetOptionNames();
  if (names.empty())
  {
    return {};
  }

  auto fold = [](std::string result, std::string name) { return std::move(result) + ", " + name; };
  std::string result = std::accumulate(std::next(names.begin()), names.end(), names[0], fold);

  if (option.IsParameter())
  {
    result += (" <" + GetValueName(option) + ">");
  }

  return result;
}

std::string GetUsageString(const std::string &app_name,
                           const std::vector<const CommandLineOption *> &options)
{
  std::string header =
      "Usage: " + app_name + " " + GetAvailableOptionsSummaryString(options) + "\n\n";

  if (options.empty())
  {
    return header;
  }

  std::string body("Options:\n");
  for (const auto option : options)
  {
    auto option_string = GetOptionUsageString(*option);
    option_string.resize(kDesiredOptionStringLength, ' ');
    body += option_string + option->GetDescription() + "\n";
  }
  return header + body;
}

}  // namespace cli
}  // namespace sup
