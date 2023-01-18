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
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_XML_EXCEPTIONS_H_
#define SUP_XML_EXCEPTIONS_H_

#include <exception>
#include <string>

namespace sup
{
namespace xml
{
  /**
 * @brief Verify the passed pointer is not null.
 *
 * @return Given pointer if it's not a nullptr.
 *
 * @throws Given exception if provided pointer was null.
 */
template <typename T, typename E>
T* AssertNoNullptr(T* ptr, E exception)
{
  if (ptr == nullptr)
  {
    throw exception;
  }
  return ptr;
}

  /**
 * @brief Base Exception class with message.
 */
class MessageException : public std::exception
{
public:
  MessageException(std::string message);
  const char* what() const noexcept override;
private:
  std::string message;
};

/**
 * @brief Exception thrown when trying to call an operation that is not supported by the object.
 */
class InvalidOperationException : public MessageException
{
public:
  InvalidOperationException(const std::string& message);
};

/**
 * @brief Exception thrown when validation of an object failed.
 */
class ValidationException : public MessageException
{
public:
  ValidationException(const std::string& message);
};

/**
 * @brief Exception thrown when parsing to an object failed.
 */
class ParseException : public MessageException
{
public:
  ParseException(const std::string& message);
};

/**
 * @brief Exception thrown when serializing an object failed.
 */
class SerializeException : public MessageException
{
public:
  SerializeException(const std::string& message);
};

}  // namespace xml

}  // namespace sup

#endif  // SUP_XML_EXCEPTIONS_H_
