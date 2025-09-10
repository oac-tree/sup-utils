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
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include "command_line_parser.h"

#include "argh.h"
#include "command_line_utils.h"

#include <algorithm>
#include <memory>
#include <string>

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

  CommandLineParserImpl();

  //! Returns true if parameter options with given name was set in command line and parameter can be
  //! parsed.
  bool IsParameterSet(const std::string &option_name) const;

  //! Returns true if flag with given name was set in comand line.
  bool IsFlagSet(const std::string &option_name) const;

  //! Returns true if option is properly set.
  bool IsFlagOptionSet(const CommandLineOption &option) const;

  //! Returns true if option is properly set.
  bool IsParameterOptionSet(const CommandLineOption &option) const;

  //! Returns true if option is properly set.
  bool IsSet(const CommandLineOption &option) const;

  std::vector<const CommandLineOption *> GetOptions();

  //! Returns true if option is required and valid.
  bool IsValidRequiredOption(const CommandLineOption &option) const;

  //! Returns true if necessary parameter arguments have been provided.
  bool IsParameterArgumentsProvided(const CommandLineOption &option) const;

  bool IsValidParsing() const;
};

CommandLineParser::CommandLineParser() : p_impl(std::make_unique<CommandLineParserImpl>()) {}

CommandLineParser::~CommandLineParser() = default;

CommandLineOption &CommandLineParser::AddOption(const std::vector<std::string> &option_names,
                                                const std::string &description)
{
  (void)p_impl->m_options.emplace_back(std::make_unique<CommandLineOption>(option_names));
  auto &result = *p_impl->m_options.back();
  (void)result.SetDescription(description);
  return result;
}

CommandLineOption &CommandLineParser::AddHelpOption()
{
  return AddOption({"-h", "--help"}).SetDescription("Displays help on command line options");
}

void CommandLineParser::AddPositionalOption(const std::string &option_name,
                                            const std::string &description)
{
  (void)AddOption({option_name}).SetDescription(description).SetPositional(true);
}

CommandLineOption *CommandLineParser::GetOption(const std::string &option_name) const
{
  for (auto& option : p_impl->m_options)
  {
    if (Contains(option->GetOptionNames(), option_name))
    {
      return option.get();
    }
  }
  return nullptr;
}

bool CommandLineParser::Parse(int32 argc, const char* const* argv)
{
  for (auto &option : p_impl->m_options)
  {
    // Underlying parser from the Argh library requires parameters (i.e. options with values) to
    // be registered before parsing.
    if (option->IsParameter())
    {
      const auto option_names = option->GetOptionNames();
      for (const auto& option_name : option_names)
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

int32 CommandLineParser::GetPositionalOptionCount() const
{
  int32 parser_result = static_cast<int32>(p_impl->m_parser.size());
  // Argh library always treats program name as a positional option and returns 1 event if no
  // positional options are present.
  return parser_result - 1;
}

std::vector<std::string> CommandLineParser::GetPositionalValues() const
{
  if (p_impl->m_parser.size() > 0)
  {
    // if parsing took place, we have to exclude program name from positional list
    auto pars_args = p_impl->m_parser.pos_args();
    return std::vector<std::string>(std::next(pars_args.begin()), pars_args.end());
  }

  return {};
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
  std::string parse_result;

  if (auto option = GetOption(option_name))
  {
    for (const auto& alias_name : option->GetOptionNames())
    {
      if ((p_impl->m_parser(alias_name) >> parse_result))
      {
        return std::stringstream(parse_result);
      }
    }
    if (!option->GetDefaultValue().empty())
    {
      return std::stringstream(option->GetDefaultValue());
    }
  }

  throw std::runtime_error("Can't parse the value");
}

std::stringstream CommandLineParser::GetPositionalValueStream(size_t index) const
{
  if (static_cast<int32>(index) >= GetPositionalOptionCount())
  {
    throw std::runtime_error("Positional index exceeds number of positional options found");
  }
  return std::stringstream(GetPositionalValues().at(index));
}

CommandLineParser::CommandLineParserImpl::CommandLineParserImpl()
  : m_options()
  , m_parser()
  , m_header()
  , m_footer()
{}

bool CommandLineParser::CommandLineParserImpl::IsParameterSet(const std::string &option_name) const
{
  std::string result = m_parser(option_name).str();
  return !result.empty();
}

bool CommandLineParser::CommandLineParserImpl::IsFlagSet(const std::string &option_name) const
{
  return m_parser[option_name];
}

bool CommandLineParser::CommandLineParserImpl::IsFlagOptionSet(
  const CommandLineOption &option) const
{
  auto names = option.GetOptionNames();
  auto predicate = [this](const std::string &str) { return IsFlagSet(str); };
  return std::find_if(names.begin(), names.end(), predicate) != names.end();
}

bool CommandLineParser::CommandLineParserImpl::IsParameterOptionSet(
  const CommandLineOption &option) const
{
  if (IsFlagOptionSet(option) && !option.GetDefaultValue().empty())
  {
    return true;
  }
  auto names = option.GetOptionNames();
  auto predicate = [this](const std::string &str) { return IsParameterSet(str); };
  return std::find_if(names.begin(), names.end(), predicate) != names.end();
}

bool CommandLineParser::CommandLineParserImpl::IsSet(const CommandLineOption &option) const
{
  return option.IsParameter() ? IsParameterOptionSet(option) : IsFlagOptionSet(option);
}

std::vector<const CommandLineOption *> CommandLineParser::CommandLineParserImpl::GetOptions()
{
  std::vector<const CommandLineOption *> result;
  (void)std::transform(m_options.begin(), m_options.end(), std::back_inserter(result),
                       [](std::unique_ptr<::sup::cli::CommandLineOption> &it)
                       { return it.get(); });
  return result;
}

bool CommandLineParser::CommandLineParserImpl::IsValidRequiredOption(
  const CommandLineOption &option) const
{
  return option.IsRequired() ? IsSet(option) : true;
}

bool CommandLineParser::CommandLineParserImpl::IsParameterArgumentsProvided(
  const CommandLineOption &option) const
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

bool CommandLineParser::CommandLineParserImpl::IsValidParsing() const
{
  for (auto& option : m_options)
  {
    if ((!IsParameterArgumentsProvided(*option)) || (!IsValidRequiredOption(*option)))
    {
      return false;
    }
  }
  return true;
}

}  // namespace cli
}  // namespace sup
