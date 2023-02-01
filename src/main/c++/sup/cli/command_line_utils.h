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

#ifndef SUP_CLI_COMMAND_LINE_UTILS_H_
#define SUP_CLI_COMMAND_LINE_UTILS_H_

#include <string>
#include <vector>

namespace sup
{
namespace cli
{

class CommandLineOption;

/**
 * Returns summary string representing all options.
 * Example: "[-h, --help] [--font=<value>] [--verbose] <positional_option>"
 */
std::string GetAvailableOptionsSummaryString(const std::vector<const CommandLineOption*>& options);

/**
 * Returns a string describing the usage of the option.
 * Example: "-v, --version"
 * Example: "-f, --file <filename>"
 */
std::string GetOptionUsageString(const CommandLineOption& option);

/**
 * Returns a string describing the program usage.
 */
std::string GetUsageString(const std::string& app_name,
                           const std::vector<const CommandLineOption*>& options);

}  // namespace cli

}  // namespace sup

#endif  // SUP_CLI_COMMAND_LINE_OPTION_H_
