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
#include "command_line_utils.h"

#include <algorithm>
#include <iostream>

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

  //! Returns true if parameter options with given name was set in command line and parameter can be
  //! parsed.
  bool IsParameterSet(const std::string &option_name)
  {
    std::string result;
    return static_cast<bool>(m_parser(option_name) >> result);
  }

  //! Returns true if flag with given name was set in comand line.
  bool IsFlagSet(const std::string &option_name) { return m_parser[option_name]; }

  //! Returns true if option is properly set.
  bool IsSet(const CommandLineOption &option)
  {
    bool result{false};
    for (const auto &option_name : option.GetOptionNames())
    {
      result |= (option.IsParameter() ? IsParameterSet(option_name) : IsFlagSet(option_name));
    }

    return result;
  }

  std::vector<const CommandLineOption *> GetOptions()
  {
    std::vector<const CommandLineOption *> result;
    std::transform(m_options.begin(), m_options.end(), std::back_inserter(result),
                   [](const auto &it) { return it.get(); });
    return result;
  }

  //! Returns true if option is required and valid.
  bool IsValidRequiredOption(const CommandLineOption &option)
  {
    return option.IsRequired() ? IsSet(option) : true;
  }

  //! Returns true if parameter option is properly set.
  bool IsValidParameterOption(const CommandLineOption &option)
  {
    if (option.IsParameter())
    {
      for (const auto &option_name : option.GetOptionNames())
      {
        if (IsFlagSet(option_name))
        {
          // appearance as a flag means that no proper parameter has been given
          return false;
        }
      }
    }
    return true;
  }

  bool IsValidParsing()
  {
    bool result{true};
    for (auto &option : m_options)
    {
      if (!IsValidParameterOption(*option) || !IsValidRequiredOption(*option))
      {
        return false;
      }
    }

    return true;
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

CommandLineOption *CommandLineParser::AddHelpOption()
{
  return AddOption({"-h", "--help"})->SetDescription("Displays help on command line options");
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

bool CommandLineParser::Parse(int argc, const char *const argv[])
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

  return !IsSet("--help") && p_impl->IsValidParsing();
}

bool CommandLineParser::IsSet(const std::string &option_name)
{
  auto option = GetOption(option_name);
  return option ? p_impl->IsSet(*option) : false;
}

std::string CommandLineParser::GetUsageString() const
{
  std::string app_name;
  p_impl->m_parser(0) >> app_name;
  return ::sup::cli::GetUsageString(app_name, p_impl->GetOptions());
}

std::stringstream CommandLineParser::GetValueStream(const std::string &option_name) const
{
  std::string str;
  if (!(p_impl->m_parser(option_name) >> str))
  {
    throw std::runtime_error("Can't parse the value");
  }
  std::stringstream result(str);

  return result;
}

}  // namespace cli
}  // namespace sup
