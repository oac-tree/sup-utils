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

#ifndef SUP_CLI_COMMAND_LINE_PARSER_H_
#define SUP_CLI_COMMAND_LINE_PARSER_H_

#include <sup/cli/command_line_option.h>

#include <memory>
#include <sstream>

namespace sup
{
namespace cli
{

class CommandLineParser
{
public:
  CommandLineParser();
  ~CommandLineParser();

  /**
   * Adds the new option to the list of available options and returns the result.
   */
  CommandLineOption* AddOption(const std::vector<std::string>& option_names);

  /**
   * Adds help option `-h, --help` to the list of available options.
   */
  CommandLineOption* AddHelpOption();

  CommandLineOption* GetOption(const std::string& option_name);

  /**
   * Parses command line arguments and returns true in the case of success.
   * @note The appearance of help option will lead to `false`.
   */
  bool Parse(int argc, const char* const argv[]);

  /**
   * Check whether \a option_name was passed to the application.
   * In the case of `parameter option` it means that the parameter after the option exists and can
   * be parsed.
   */
  bool IsSet(const std::string& option_name);

  template <typename T>
  T GetValue(const std::string& option_name) const;

  /**
   * Returns usage string.
   */
  std::string GetUsageString() const;

private:
  std::stringstream GetValueStream(const std::string& option_name) const;

  struct CommandLineParserImpl;
  std::unique_ptr<CommandLineParserImpl> p_impl;
};

template <typename T>
T CommandLineParser::GetValue(const std::string& option_name) const
{
  T result;
  GetValueStream(option_name) >> result;
  return result;
}

}  // namespace cli

}  // namespace sup

#endif  // SUP_CLI_COMMAND_LINE_OPTION_H_
