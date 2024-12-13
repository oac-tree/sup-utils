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
#include "base_types.h"

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
 * There are three types of options:
 *  1) Flags
 *     Command line arguments beginning with "-", e.g.: -v, --verbose.
 *     Represent a boolean.
 *  2) Parameters
 *     A named value followed by a non-options.
 *     e.g.: --font 24
 *  3) Positional
 *     Free standing values, e.g: config.json
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
   * @brief Adds the new option to the list of available options and returns the result.
   *
   * Option name should start from a dash for flags and parameter options. For positional options
   * use AddPositionalOption method.
   *
   * @param option_names List of option alias as it will appear in the command line.
   * @param description Option description
   * @return Created option.
   *
   * @details A help option can be defined as {"-h", "--help"}
   */
  CommandLineOption& AddOption(const std::vector<std::string>& option_names,
                               const std::string& description = {});

  /**
   * @brief Adds help option `-h, --help` to the list of available options.
   *
   * Help info will be automatically generated from the list of created options.
   */
  CommandLineOption& AddHelpOption();

  /**
   * @brief Adds positional option.
   *
   * The name of the positional option is used only for the help string. It shouldn't appear in the
   * command line.
   *
   * @param option_name Option name.
   * @param description Option description.
   */
  void AddPositionalOption(const std::string& option_name, const std::string& description = {});

  /**
   * @brief Returns option with given name.
   */
  CommandLineOption* GetOption(const std::string& option_name) const;

  /**
   * @brief Parses command line arguments.
   * @return True in the case of success.
   */
  bool Parse(int32 argc, const char* const* argv);

  /**
   * @brief Checks whether option_name was passed to the application.
   *
   * In the case of parameter option it means that the parameter after the option exists and can be
   * parsed.
   */
  bool IsSet(const std::string& option_name);

  /**
   * @brief Returns the value of an option.
   */
  template <typename T>
  T GetValue(const std::string& option_name) const;

  /**
   * @brief Returns a number of positional options found after the parsing.
   *
   * The name of a program (which always appears as a first element of *argv array) is not
   * considered as a positional option. The method will return -1 if parsing doesn't have a place
   * yet.
   */
  int32 GetPositionalOptionCount() const;

  /**
   * @brief Returns list of positional options found.
   */
  std::vector<std::string> GetPositionalValues() const;

  /**
   * @brief Returns a positional value found at the given index in the list of positional values.
   *
   * Will throw, if index exceeds a number of positional values found.
   */
  template <typename T>
  T GetPositionalValue(size_t index) const;

  /**
   * @brief Returns multi-line string.
   */
  std::string GetUsageString() const;

  /**
   * @brief Adds header and footer text to the usage multiline string.
   *
   * These will appear before and after option description.
   */
  void SetDescription(const std::string& header, const std::string& footer);

private:
  std::stringstream GetValueStream(const std::string& option_name) const;

  std::stringstream GetPositionalValueStream(size_t index) const;

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

template <typename T>
T CommandLineParser::GetPositionalValue(size_t index) const
{
  T result;
  GetPositionalValueStream(index) >> result;
  return result;
}

}  // namespace cli

}  // namespace sup

#endif  // SUP_CLI_COMMAND_LINE_OPTION_H_
