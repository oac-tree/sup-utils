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
 * Copyright (c) : 2010-2026 ITER Organization,
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
 * @brief Returns list of options filtered according positional flag.
 */
const std::vector<const CommandLineOption*> GetFilteredOptions(
    const std::vector<const CommandLineOption*>& options, bool positional_flag);

/**
 * @brief Returns a string that is the result of a string concatenation, with a new line inserted
 * after each non-empty string.
 */
std::string MergeWithNewLine(std::initializer_list<std::string> strings);

/**
 * Returns summary string representing all options.
 * Example: "[-h, --help] [--font=<value>] [--verbose] <positional_option>"
 */
std::string GetAvailableOptionsSummaryString(const std::vector<const CommandLineOption*>& options);

/**
 * @brief Returns a string giving a summary on option names.
 * Example: "-v, --version"
 * Example: "-f, --file <filename>"
 */
std::string GetOptionNameString(const CommandLineOption& option);

/**
 * @brief Returns a string describing the usage.
 *
 * "--version           software version"
 */
std::string GetOptionUsageString(const CommandLineOption& option);

/**
 * @brief Returns a multiline string describing the usage of all options.
 * "--version           software version"
 * "-v, --verbose       verbose level"
 */
std::string GetOptionBlockString(const std::vector<const CommandLineOption*>& options);

/**
 * @brief Returns a string describing the program usage.
 */
std::string GetUsageString(const std::string& app_name, const std::string& header,
                           const std::vector<const CommandLineOption*>& options,
                           const std::string& footer);

}  // namespace cli

}  // namespace sup

#endif  // SUP_CLI_COMMAND_LINE_OPTION_H_
