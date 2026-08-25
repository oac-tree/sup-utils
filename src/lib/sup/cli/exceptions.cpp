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

#include "exceptions.h"

namespace sup
{
namespace cli
{

CommandLineException::CommandLineException(const std::string& message) : std::runtime_error{message}
{
}

InvalidOptionException::InvalidOptionException(const std::string& message)
    : CommandLineException{message}
{
}

InvalidValueException::InvalidValueException(const std::string& message)
    : CommandLineException{message}
{
}

MissingValueException::MissingValueException(const std::string& message)
    : CommandLineException{message}
{
}

}  // namespace cli

}  // namespace sup
