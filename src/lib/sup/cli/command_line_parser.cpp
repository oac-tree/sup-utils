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
#include <sup/cli/command_line_option.h>

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
    (void)std::transform(m_options.begin(), m_options.end(), std::back_inserter(result),
                         [](std::unique_ptr<::sup::cli::CommandLineOption> &it)
                         { return it.get(); });
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
    for (const std::unique_ptr<CommandLineOption> &option : m_options)
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
  for (const std::unique_ptr<CommandLineOption> &option : p_impl->m_options)
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
      for (const std::string &option_name : option->GetOptionNames())
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

int CommandLineParser::GetPositionalOptionCount() const
{
  int parser_result = static_cast<int>(p_impl->m_parser.size());
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
    for (const std::string &alias_name : option->GetOptionNames())
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
  if (static_cast<int>(index) >= GetPositionalOptionCount())
  {
    throw std::runtime_error("Positional index exceeds number of positional options found");
  }
  return std::stringstream(GetPositionalValues().at(index));
}

}  // namespace cli
}  // namespace sup
