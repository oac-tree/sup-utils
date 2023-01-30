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

#include "command_line_option.h"

namespace sup
{
namespace cli
{

CommandLineOption::CommandLineOption(const std::string &option_name)
    : CommandLineOption(std::vector<std::string>{option_name})
{
}

CommandLineOption::CommandLineOption(const std::vector<std::string> &option_names)
    : m_option_names(option_names), m_default_value()
{
}

CommandLineOption *CommandLineOption::SetDefaultValue(const std::string &default_value)
{
  m_default_value = default_value;
  return this;
}

}  // namespace cli
}  // namespace sup
