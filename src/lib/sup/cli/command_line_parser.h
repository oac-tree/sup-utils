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

namespace sup
{
namespace cli
{

class CommandLineParser
{
public:
  CommandLineParser();
  ~CommandLineParser();

  CommandLineOption* AddOption(const std::vector<std::string>& option_names);

  CommandLineOption* GetOption(const std::string& option_name);

  void Parse(int argc, const char* const argv[]);

  bool IsSet(const std::string& option_name);

private:
  struct CommandLineParserImpl;
  std::unique_ptr<CommandLineParserImpl> p_impl;
};

}  // namespace cli

}  // namespace sup

#endif  // SUP_CLI_COMMAND_LINE_OPTION_H_
