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

#ifndef SUP_CLI_COMMAND_LINE_OPTION_H_
#define SUP_CLI_COMMAND_LINE_OPTION_H_

#include <string>
#include <vector>

namespace sup
{
namespace cli
{

/**
 * The CommandLineOption class defines a possible command-line option. It doesn't contain the
 * results of command line parsing.
 *
 * Command line option is defined by its option names, as it appears in the command line. For
 * example, a help option can be defined as {"-h", "--help"}.
 *
 * Options are divided into three types: flags, flags parameters, and positional options.
 *
 * There are three types of options:
 *  1) Flags
 *     Command line arguments beginning with "-", e.g.: -v, --verbose.
 *     Represent a boolean.
 *  2) Parameters
 *     A named value followed by a non-options.
 *     e.g.: --font 24
 *  3) Positional
 *     Three standing values, e.g: config.json
 *
 * For positional options, the option names only appear in the description, and shouldn't appear in
 * command line.
 *
 * For the moment, the class doesn't have consistency check (option can be both positional and
 * parameter options).
 */
class CommandLineOption
{
public:
  /**
   * @brief Main constructor.
   *
   * @param option_names Option names.
   */
  CommandLineOption(const std::vector<std::string>& option_names);

  /**
   * @brief Returns option names.
   */
  std::vector<std::string> GetOptionNames() const;

  /**
   * @brief Returns the default value.
   */
  std::string GetDefaultValue() const;

  /**
   * @brief Sets the default value.
   */
  CommandLineOption& SetDefaultValue(const std::string& default_value);

  /**
   * @brief Sets option description.
   */
  std::string GetDescription() const;

  /**
   * @brief Returns option description.
   */
  CommandLineOption& SetDescription(const std::string& description);

  /**
   * @brief Checks if the option is required.
   */
  bool IsRequired() const;

  /**
   * @brief Sets requirement flag to a given value.
   */
  CommandLineOption& SetRequired(bool value);

  /**
   * @brief Returns the name of the value.
   *
   * It is a text which will be used to form help line. By default, the name "[value]" is used.
   */
  std::string GetValueName() const;

  /**
   * @brief Sets the name to a given value.
   */
  CommandLineOption& SetValueName(const std::string& value_name);

  /**
   * @brief Checks if option is positional.
   */
  bool IsPositional() const;

  /**
   * @brief Set the positional flag to a given value.
   */
  CommandLineOption& SetPositional(bool value);

  /**
   * @brief Checks if given option is a flag with required parameter.
   */
  bool IsParameter() const;

  /**
   * @brief Sets parameter flag to a given value.
   */
  CommandLineOption& SetParameter(bool value);

private:
  std::vector<std::string> m_option_names;
  std::string m_default_value;
  std::string m_description;
  std::string m_value_name;  //!< the text representing the value for help string
  bool m_is_required;
  bool m_is_positional;
  bool m_is_parameter;
};

}  // namespace cli

}  // namespace sup

#endif  // SUP_CLI_COMMAND_LINE_OPTION_H_
