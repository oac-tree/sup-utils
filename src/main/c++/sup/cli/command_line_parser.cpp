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

#include "command_line_parser.h"

#include <algorithm>

namespace
{
template <typename A, typename B>
bool Contains(const A &container, const B &element)
{
  return std::find(container.begin(), container.end(), element) != container.end();
}
}  // namespace

namespace sup
{
namespace cli
{

struct CommandLineParser::CommandLineParserImpl
{
  std::vector<std::unique_ptr<CommandLineOption>> m_options;

  CommandLineParserImpl() : m_options() {}
};

CommandLineParser::CommandLineParser() : p_impl(std::make_unique<CommandLineParserImpl>()) {}

CommandLineParser::~CommandLineParser() = default;

CommandLineOption *CommandLineParser::AddOption(const std::vector<std::string> &option_names)
{
  p_impl->m_options.emplace_back(std::make_unique<CommandLineOption>(option_names));
  return p_impl->m_options.back().get();
}

CommandLineOption *CommandLineParser::GetOption(const std::string &option_name)
{
  for (auto &option : p_impl->m_options)
  {
    if (Contains(option->GetOptionNames(), option_name))
    {
      return option.get();
    }
  }
  return nullptr;
}

}  // namespace cli
}  // namespace sup
