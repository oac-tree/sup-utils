/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : Supervision and Automation System Utilities
 *
 * Description   : SUP XML utilities
 *
 * Author        : Walter Van Herck (IO)
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

#include "exceptions.h"

namespace sup
{
namespace xml
{

MessageException::MessageException(const std::string& message)
  : std::exception(), m_message{std::move(message)}
{}

const char* MessageException::what() const noexcept
{
  return m_message.c_str();
}

InvalidOperationException::InvalidOperationException(const std::string& message)
  : MessageException{message}
{}

ValidationException::ValidationException(const std::string& message)
  : MessageException{message}
{}

ParseException::ParseException(const std::string& message)
  : MessageException{message}
{}

SerializeException::SerializeException(const std::string& message)
  : MessageException{message}
{}

}  // namespace xml

}  // namespace sup
