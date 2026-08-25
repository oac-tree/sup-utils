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

#include "command_line_parser.h"

#include "argh.h"
#include "command_line_utils.h"
#include "exceptions.h"

#include <algorithm>
#include <cstdint>
#include <memory>
#include <string>

namespace
{
template <typename A, typename B>
bool Contains(const A& container, const B& element)
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

  //! Returns option with given name, or nullptr if no such option was added. The constness of the
  //! result is decided by the caller: the lookup itself doesn't modify the option.
  CommandLineOption* FindOption(const std::string& option_name) const;

  //! Returns true if parameter options with given name was set in command line and parameter can be
  //! parsed.
  bool IsParameterSet(const std::string& option_name) const;

  //! Returns true if flag with given name was set in comand line.
  bool IsFlagSet(const std::string& option_name) const;

  //! Returns true if option is properly set.
  bool IsFlagOptionSet(const CommandLineOption& option) const;

  //! Returns true if option is properly set.
  bool IsParameterOptionSet(const CommandLineOption& option) const;

  //! Returns true if option is properly set.
  bool IsSet(const CommandLineOption& option) const;

  std::vector<const CommandLineOption*> GetOptions();

  //! Returns true if option is required and valid.
  bool IsValidRequiredOption(const CommandLineOption& option) const;

  //! Returns true if necessary parameter arguments have been provided.
  bool IsParameterArgumentsProvided(const CommandLineOption& option) const;

  bool IsValidParsing() const;
};

CommandLineParser::CommandLineParser() : p_impl(std::make_unique<CommandLineParserImpl>()) {}

CommandLineParser::~CommandLineParser() = default;

// Defined here rather than in the header: defaulting these requires CommandLineParserImpl to be a
// complete type.
CommandLineParser::CommandLineParser(CommandLineParser&& other) noexcept = default;

CommandLineParser& CommandLineParser::operator=(CommandLineParser&& other) & noexcept = default;

CommandLineOption& CommandLineParser::AddOption(const std::vector<std::string>& option_names,
                                                const std::string& description)
{
  (void)p_impl->m_options.emplace_back(std::make_unique<CommandLineOption>(option_names));
  auto& result = *p_impl->m_options.back();
  (void)result.SetDescription(description);
  return result;
}

CommandLineOption& CommandLineParser::AddHelpOption()
{
  return AddOption({"-h", "--help"}).SetDescription("Displays help on command line options");
}

void CommandLineParser::AddPositionalOption(const std::string& option_name,
                                            const std::string& description)
{
  (void)AddOption({option_name}).SetDescription(description).SetPositional(true);
}

CommandLineOption* CommandLineParser::GetOption(const std::string& option_name)
{
  return p_impl->FindOption(option_name);
}

const CommandLineOption* CommandLineParser::GetOption(const std::string& option_name) const
{
  return p_impl->FindOption(option_name);
}

bool CommandLineParser::Parse(std::int32_t argc, const char* const* argv)
{
  for (auto& option : p_impl->m_options)
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

bool CommandLineParser::IsSet(const std::string& option_name) const
{
  auto option = GetOption(option_name);
  return option ? p_impl->IsSet(*option) : false;
}

std::int32_t CommandLineParser::GetPositionalOptionCount() const
{
  const auto parser_result = static_cast<std::int32_t>(p_impl->m_parser.size());
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

void CommandLineParser::SetDescription(const std::string& header, const std::string& footer)
{
  p_impl->m_header = header;
  p_impl->m_footer = footer;
}

std::optional<std::string> CommandLineParser::FindValue(const std::string& option_name) const
{
  const auto* option = GetOption(option_name);
  if (option == nullptr)
  {
    throw InvalidOptionException("Option '" + option_name
                                 + "' is not among the options added to the parser");
  }

  for (const auto& alias_name : option->GetOptionNames())
  {
    // The state of the stream is the presence test: an option that was not given yields a stream
    // in a failed state, while one given an empty value yields a good, empty one. Extracting from
    // it would serve neither purpose, since extraction fails on an empty value and stops at the
    // first whitespace of a value containing spaces.
    auto value_stream = p_impl->m_parser(alias_name);
    if (value_stream)
    {
      return value_stream.str();
    }
  }

  if (option->HasDefaultValue())
  {
    return option->GetDefaultValue();
  }

  return {};
}

std::string CommandLineParser::GetValueAsText(const std::string& option_name) const
{
  const auto result = FindValue(option_name);
  if (!result.has_value())
  {
    throw MissingValueException("Option '" + option_name
                                + "' was not given in the command line and has no default value");
  }
  return *result;
}

std::string CommandLineParser::GetPositionalValueAsText(size_t index) const
{
  const auto values = GetPositionalValues();
  if (index >= values.size())
  {
    throw MissingValueException("Positional value with index " + std::to_string(index)
                                + " doesn't exist, the number of positional values found is "
                                + std::to_string(values.size()));
  }
  return values.at(index);
}

CommandLineParser::CommandLineParserImpl::CommandLineParserImpl()
    : m_options(), m_parser(), m_header(), m_footer()
{
}

CommandLineOption* CommandLineParser::CommandLineParserImpl::FindOption(
    const std::string& option_name) const
{
  for (const auto& option : m_options)
  {
    if (Contains(option->GetOptionNames(), option_name))
    {
      return option.get();
    }
  }
  return nullptr;
}

bool CommandLineParser::CommandLineParserImpl::IsParameterSet(const std::string& option_name) const
{
  // A value that was given counts as given even when it is empty, hence the state of the stream
  // rather than the emptiness of its contents.
  return static_cast<bool>(m_parser(option_name));
}

bool CommandLineParser::CommandLineParserImpl::IsFlagSet(const std::string& option_name) const
{
  return m_parser[option_name];
}

bool CommandLineParser::CommandLineParserImpl::IsFlagOptionSet(
    const CommandLineOption& option) const
{
  auto names = option.GetOptionNames();
  auto predicate = [this](const std::string& str) { return IsFlagSet(str); };
  return std::find_if(names.begin(), names.end(), predicate) != names.end();
}

bool CommandLineParser::CommandLineParserImpl::IsParameterOptionSet(
    const CommandLineOption& option) const
{
  if (IsFlagOptionSet(option) && option.HasDefaultValue())
  {
    return true;
  }
  auto names = option.GetOptionNames();
  auto predicate = [this](const std::string& str) { return IsParameterSet(str); };
  return std::find_if(names.begin(), names.end(), predicate) != names.end();
}

bool CommandLineParser::CommandLineParserImpl::IsSet(const CommandLineOption& option) const
{
  return option.IsParameter() ? IsParameterOptionSet(option) : IsFlagOptionSet(option);
}

std::vector<const CommandLineOption*> CommandLineParser::CommandLineParserImpl::GetOptions()
{
  std::vector<const CommandLineOption*> result;
  (void)std::transform(m_options.begin(), m_options.end(), std::back_inserter(result),
                       [](std::unique_ptr<::sup::cli::CommandLineOption>& it) { return it.get(); });
  return result;
}

bool CommandLineParser::CommandLineParserImpl::IsValidRequiredOption(
    const CommandLineOption& option) const
{
  return option.IsRequired() ? IsSet(option) : true;
}

bool CommandLineParser::CommandLineParserImpl::IsParameterArgumentsProvided(
    const CommandLineOption& option) const
{
  if (!option.IsParameter())
  {
    // non-parameters (i.e. flags) always pass
    return true;
  }
  // Special case when the user provides parameter option without the value. We check that there
  // is no flag found with the same name, which would be a sign of forgotten arguments.
  // Presence of default argument makes it always passign.
  return option.HasDefaultValue() ? true : !IsFlagOptionSet(option);
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
