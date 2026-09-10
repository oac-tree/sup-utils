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

#ifndef SUP_CLI_COMMAND_LINE_PARSER_H_
#define SUP_CLI_COMMAND_LINE_PARSER_H_

#include <sup/cli/command_line_option.h>
#include <sup/cli/exceptions.h>
#include <sup/cli/value_conversion.h>

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <memory>
#include <optional>
#include <string>

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
 * See usage examples in test/cli-example folder.
 *
 * An option that was not added to the parser is a failure and is reported by GetParseError: a
 * misspelled option is refused rather than passed over. Free standing values are not checked
 * this way, they remain available through GetPositionalValues however many of them were given.
 */
class CommandLineParser
{
public:
  CommandLineParser();
  ~CommandLineParser();

  /**
   * @brief Copy constructor and assignment (deleted).
   */
  CommandLineParser(const CommandLineParser& other) = delete;
  CommandLineParser& operator=(const CommandLineParser& other) & = delete;

  /**
   * @brief Move constructor and assignment.
   *
   * The parser that was moved from shall not be used afterwards.
   */
  CommandLineParser(CommandLineParser&& other) noexcept;
  CommandLineParser& operator=(CommandLineParser&& other) & noexcept;

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
   * @return Created option.
   *
   * @details Positional options are matched to the values found in the command line in the order
   * they were added, so marking one required means that a value has to reach it.
   */
  CommandLineOption& AddPositionalOption(const std::string& option_name,
                                         const std::string& description = {});

  /**
   * @brief Returns option with given name, or nullptr if no such option was added.
   */
  CommandLineOption* GetOption(const std::string& option_name);

  /**
   * @brief Returns option with given name, or nullptr if no such option was added.
   */
  const CommandLineOption* GetOption(const std::string& option_name) const;

  /**
   * @brief Parses command line arguments.
   * @return True in the case of success.
   */
  bool Parse(std::int32_t argc, const char* const* argv);

  /**
   * @brief Checks whether the help option was given in the command line.
   *
   * Only the option created by AddHelpOption counts as the help option. Returns false if the
   * application doesn't offer one, or if the parsing didn't take place yet.
   */
  bool IsHelpRequested() const;

  /**
   * @brief Returns the reason why the last parsing failed.
   *
   * @return A message naming the option at fault, or an empty string.
   */
  std::string GetParseError() const;

  /**
   * @brief Checks whether option_name was passed to the application.
   *
   * In the case of parameter option it means that the parameter after the option exists and can be
   * parsed.
   */
  bool IsSet(const std::string& option_name) const;

  /**
   * @brief Returns the value of an option, converted to the requested type.
   *
   * The whole text of the value is converted. The call will throw if conversion is not possible.
   *
   * @throws InvalidOptionException if no option with this name was added to the parser.
   * @throws MissingValueException if the option was not given and has no default value.
   * @throws InvalidValueException if the text of the value doesn't convert to the requested type.
   */
  template <typename T>
  T GetValue(const std::string& option_name) const;

  /**
   * @brief Returns the text of the value of an option, if there is one.
   *
   * @param option_name Name of the option.
   * @return The value as it was given in the command line, or the default value, or an empty
   * optional if the option was not given and has no default value.
   *
   * @throws InvalidOptionException if no option with this name was added to the parser.
   */
  std::optional<std::string> FindValue(const std::string& option_name) const;

  /**
   * @brief Returns the value of an option, or the given fallback if the option was not given.
   *
   * The fallback stands in for a value that is absent, and not for one that is unusable: a value
   * that was given but doesn't convert is still reported.
   *
   * @param option_name Name of the option.
   * @param fallback The value to return if the option was not given and has no default value.
   *
   * @throws InvalidOptionException if no option with this name was added to the parser.
   * @throws InvalidValueException if the text of the value doesn't convert to the requested type.
   */
  template <typename T>
  T GetValue(const std::string& option_name, const T& fallback) const;

  /**
   * @brief Returns the value of an option, checked against the given bounds.
   *
   * @param option_name Name of the option.
   * @param lowest Smallest admissible value, inclusive.
   * @param highest Largest admissible value, inclusive.
   *
   * @throws InvalidValueException if the value falls outside the bounds (NaN will throw too).
   */
  template <typename T>
  T GetValueInRange(const std::string& option_name, T lowest, T highest) const;

  /**
   * @brief Checks whether the command line was parsed.
   */
  bool HasParsed() const;

  /**
   * @brief Returns the number of positional values found after the parsing.
   *
   * The name of a program (which always appears as a first element of *argv array) is not
   * considered as a positional value. The method returns 0 if the parsing didn't take place yet.
   */
  std::size_t GetPositionalOptionCount() const;

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
  T GetPositionalValue(std::size_t index) const;

  /**
   * @brief Returns multi-line string describing all options.
   */
  std::string GetUsageString() const;

  /**
   * @brief Adds header and footer text to the usage multiline string.
   *
   * These will appear before and after option description.
   */
  void SetDescription(const std::string& header, const std::string& footer);

private:
  std::string GetValueAsText(const std::string& option_name) const;

  std::string GetPositionalValueAsText(std::size_t index) const;

  struct CommandLineParserImpl;
  std::unique_ptr<CommandLineParserImpl> p_impl;
};

template <typename T>
T CommandLineParser::GetValue(const std::string& option_name) const
{
  return ConvertValue<T>("option '" + option_name + "'", GetValueAsText(option_name));
}

template <typename T>
T CommandLineParser::GetValue(const std::string& option_name, const T& fallback) const
{
  const auto text = FindValue(option_name);
  return text.has_value() ? ConvertValue<T>("option '" + option_name + "'", *text) : fallback;
}

template <typename T>
T CommandLineParser::GetValueInRange(const std::string& option_name, T lowest, T highest) const
{
  const auto context = "option '" + option_name + "'";
  const auto text = GetValueAsText(option_name);
  const auto result = ConvertValue<T>(context, text);

  // Written as a negated conjunction on purpose: a not-a-number fails both comparisons, and is
  // reported instead of being let through.
  if (!((result >= lowest) && (result <= highest)))
  {
    ThrowInvalidValue(context, text,
                      "is outside the allowed range of " + std::to_string(lowest) + " to "
                          + std::to_string(highest));
  }

  return result;
}

template <typename T>
T CommandLineParser::GetPositionalValue(std::size_t index) const
{
  return ConvertValue<T>("positional value " + std::to_string(index),
                         GetPositionalValueAsText(index));
}

/**
 * @brief Parses the command line and reports the outcome, on behalf of main.
 *
 * Will print help/error messages if necessary. The main() should exit, if the output contains the
 * value.
 *
 * @param parser The parser with all options of the application already added.
 * @param argc Number of command line arguments, as given to main.
 * @param argv Command line arguments, as given to main.
 * @param out Stream for help output.
 * @param err Stream for the error message.
 * @return The value main should return, or an empty optional if the application should carry on.
 */
std::optional<int> ParseOrReport(CommandLineParser& parser, std::int32_t argc,
                                 const char* const* argv, std::ostream& out, std::ostream& err);

/**
 * @brief Parses the command line and reports the outcome to the standard streams.
 *
 * Same as above with std::cout and std::cerr.
 */
std::optional<int> ParseOrReport(CommandLineParser& parser, std::int32_t argc,
                                 const char* const* argv);

}  // namespace cli

}  // namespace sup

#endif  // SUP_CLI_COMMAND_LINE_PARSER_H_
