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
 * The CommandLineOption class defines a possible command-line option.
 * It doesn't contain results of command line parsing.
 */

class CommandLineOption
{
public:
  CommandLineOption(const std::vector<std::string>& option_names);

  std::vector<std::string> GetOptionNames() const;

  std::string GetDefaultValue() const;
  CommandLineOption& SetDefaultValue(const std::string& default_value);

  std::string GetDescription() const;
  CommandLineOption& SetDescription(const std::string& description);

  bool IsRequired() const;
  CommandLineOption& SetRequired(bool value);

  std::string GetValueName() const;
  CommandLineOption& SetValueName(const std::string& value_name);

  bool IsPositional() const;

  bool IsParameter() const;
  CommandLineOption& SetParameter(bool value);

private:
  std::vector<std::string> m_option_names;
  std::string m_default_value;
  std::string m_description;
  std::string m_value_name;  //!< the name of the value to appear in help string
  bool m_is_required;
  bool m_is_parameter;
};

}  // namespace cli

}  // namespace sup

#endif  // SUP_CLI_COMMAND_LINE_OPTION_H_
