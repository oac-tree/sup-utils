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

#include "argh.h"

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
  argh::parser m_parser;

  //! Returns true if option is set
  bool IsSet(const CommandLineOption *option)
  {
    if (option)
    {
      for (const auto &option_name : option->GetOptionNames())
      {
        if (option->IsParameter())
        {
          std::string result;
          if ((m_parser(option_name) >> result))
          {
            return true;
          }
        }
        else
        {
          if (m_parser[option_name])
          {
            return true;
          }
        }
      }
    }
    return false;
  }

  CommandLineParserImpl() : m_options(), m_parser() {}
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

void CommandLineParser::Parse(int argc, const char *const argv[])
{
  for (auto &option : p_impl->m_options)
  {
    // Underlying parser from the Argh library requires parameters (i.e. options with values) to
    // be registered before parsing.
    if (option->IsParameter())
    {
      for (const auto &option_name : option->GetOptionNames())
      {
        p_impl->m_parser.add_param(option_name);
      }
    }
  }

  p_impl->m_parser.parse(argc, argv);
}

bool CommandLineParser::IsSet(const std::string &option_name)
{
  auto option = GetOption(option_name);
  return p_impl->IsSet(option);
}

}  // namespace cli
}  // namespace sup
