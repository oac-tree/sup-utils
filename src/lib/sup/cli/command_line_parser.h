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

/**
 * @brief The CommandLineParser class parses the command line arguments.
 *
 * It allows forming a list of expected options, parsing command line arguments, and then querying
 * the values of the options set.
 *
 * @code
 * CommandLineParser parser;
 *
 * // adds verbose flag
 * parser.AddOption({"-v", "--verbose})->SetDescription("Verbose level");
 *
 * //
 * parser.AddOption("--file")->SetParameter(true)->SetRequired(true);
 *
 * assert(parser.parse(argc, argv));
 *
 * std::cout << "Verbosity:" << parser.IsSet("-v");
 * std::cout << "Filename:" << parser.GetValue<std::string>("--file");

 * @endcode
 */
class CommandLineParser
{
public:
  CommandLineParser();
  ~CommandLineParser();

  /**
   * Adds the new option to the list of available options and returns the result.
   * Option name should start from a dash for flags and parameter options, otherwise, it will be
   * treated as a positional argument.
   */
  CommandLineOption& AddOption(const std::vector<std::string>& option_names,
                               const std::string& description = {});

  /**
   * Adds help option `-h, --help` to the list of available options.
   */
  CommandLineOption& AddHelpOption();

  CommandLineOption* GetOption(const std::string& option_name) const;

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
