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
  std::string m_header;
  std::string m_footer;

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
  bool IsFlagOptionSet(const CommandLineOption &option)
  {
    auto names = option.GetOptionNames();
    auto predicate = [this](const std::string &str) { return IsFlagSet(str); };
    return std::find_if(names.begin(), names.end(), predicate) != names.end();
  }

  //! Returns true if option is properly set.
  bool IsParameterOptionSet(const CommandLineOption &option)
  {
    if (IsFlagOptionSet(option) && !option.GetDefaultValue().empty())
    {
      return true;
    }

    auto names = option.GetOptionNames();
    auto predicate = [this](const std::string &str) { return IsParameterSet(str); };
    return std::find_if(names.begin(), names.end(), predicate) != names.end();
  }

  //! Returns true if option is properly set.
  bool IsSet(const CommandLineOption &option)
  {
    return option.IsParameter() ? IsParameterOptionSet(option) : IsFlagOptionSet(option);
  }

  std::vector<const CommandLineOption *> GetOptions()
  {
    std::vector<const CommandLineOption *> result;
    std::transform(m_options.begin(), m_options.end(), std::back_inserter(result),
                   [](std::unique_ptr<::sup::cli::CommandLineOption> &it) { return it.get(); });
    return result;
  }

  //! Returns true if option is required and valid.
  bool IsValidRequiredOption(const CommandLineOption &option)
  {
    return option.IsRequired() ? IsSet(option) : true;
  }

  //! Returns true if necessary parameter arguments have been provided.
  bool IsParameterArgumentsProvided(const CommandLineOption &option)
  {
    if (!option.IsParameter())
    {
      // non-parameters (i.e. flags) always pass
      return true;
    }

    // Special case when the user provides parameter option without the value. We check that there
    // is no flag found with the same name, which would be a sign of forgotten arguments.
    // Presence of default argument makes it always passign.
    return option.GetDefaultValue().empty() ? !IsFlagOptionSet(option) : true;
  }

  bool IsValidParsing()
  {
    bool result{true};
    for (auto &option : m_options)
    {
      result &= (IsParameterArgumentsProvided(*option) && IsValidRequiredOption(*option));
    }

    return result;
  }

  CommandLineParserImpl() : m_options(), m_parser(), m_header(), m_footer() {}
};

CommandLineParser::CommandLineParser() : p_impl(new CommandLineParserImpl) {}

CommandLineParser::~CommandLineParser() = default;

CommandLineOption &CommandLineParser::AddOption(const std::vector<std::string> &option_names,
                                                const std::string &description)
{
  p_impl->m_options.emplace_back(new CommandLineOption(option_names));
  auto &result = *p_impl->m_options.back();
  result.SetDescription(description);
  return result;
}

CommandLineOption &CommandLineParser::AddHelpOption()
{
  return AddOption({"-h", "--help"}).SetDescription("Displays help on command line options");
}

CommandLineOption *CommandLineParser::GetOption(const std::string &option_name) const
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
  return ::sup::cli::GetUsageString(app_name, p_impl->m_header, p_impl->GetOptions(),
                                    p_impl->m_footer);
}

void CommandLineParser::SetDescription(const std::string &header, const std::string &footer)
{
  p_impl->m_header = header;
  p_impl->m_footer = footer;
}

std::stringstream CommandLineParser::GetValueStream(const std::string &option_name) const
{
  std::string str;
  auto option = GetOption(option_name);

  std::string default_value = (option ? option->GetDefaultValue() : std::string());

  if (!(p_impl->m_parser(option_name, default_value) >> str))
  {
    throw std::runtime_error("Can't parse the value");
  }
  std::stringstream result(str);

  return result;
}

}  // namespace cli
}  // namespace sup
