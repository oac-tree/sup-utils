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

#include <algorithm>
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

const std::vector<const CommandLineOption *> GetFilteredOptions(
    const std::vector<const CommandLineOption *> &options, bool positional_flag)
{
  std::vector<const CommandLineOption *> result;
  auto on_options = [positional_flag](const sup::cli::CommandLineOption *option)
  { return option->IsPositional() == positional_flag; };
  std::copy_if(std::begin(options), std::end(options), std::back_inserter(result), on_options);
  return result;
}

std::string MergeWithNewLine(std::initializer_list<std::string> strings)
{
  std::string str;

  auto pack = [](std::string result, std::string element)
  { return element.empty() ? result : result + element + "\n"; };
  return std::accumulate(strings.begin(), strings.end(), str, pack);
}

std::string GetAvailableOptionsSummaryString(const std::vector<const CommandLineOption *> &options)
{
  (void)options;
  // currently returns simplified representation of all options
  return {"[options]"};
}

std::string GetOptionNameString(const CommandLineOption &option)
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

std::string GetOptionUsageString(const CommandLineOption &option)
{
  auto result = GetOptionNameString(option);
  result.resize(kDesiredOptionStringLength, ' ');
  return result + option.GetDescription();
}

std::string GetUsageString(const std::string &app_name, const std::string &header,
                           const std::vector<const CommandLineOption *> &options,
                           const std::string &footer)
{
  std::string usage_short =
      "Usage: " + app_name + " " + GetAvailableOptionsSummaryString(options) + "\n";

  return MergeWithNewLine({usage_short, header, GetOptionBlockString(options), footer});
}

std::string GetOptionBlockString(const std::vector<const CommandLineOption *> &options)
{
  if (options.empty())
  {
    return {};
  }

  std::string result("Options:\n");
  for (const auto option : GetFilteredOptions(options, /*positional*/ false))
  {
    result += GetOptionUsageString(*option) + "\n";
  }
  return result;
}

}  // namespace cli
}  // namespace sup
