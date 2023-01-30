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

#ifndef SUP_CLI_COMMAND_LINE_OPTIONS_H_
#define SUP_CLI_COMMAND_LINE_OPTIONS_H_

#include <string>
#include <vector>

namespace sup
{
namespace cli
{

class CommandLineOption
{
public:

  CommandLineOption(const std::string& option_name);

  CommandLineOption(const std::vector<std::string>& option_names);

  std::vector<std::string> GetOptionNames() const;

  CommandLineOption* SetDefaultValue(const std::string& default_value);

private:
  std::vector<std::string> m_option_names;
  std::string m_default_value;
};

}  // namespace cli

}  // namespace sup

#endif  // SUP_CLI_COMMAND_LINE_OPTIONS_H_
