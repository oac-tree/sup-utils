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
// const int kDesiredOptionStringLength = 20;
// const int kDesiredDescriptionStringLength = 60;

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

}  // namespace cli
}  // namespace sup
