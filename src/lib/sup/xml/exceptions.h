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
  explicit MessageException(std::string message);
  ~MessageException() override = default;

  const char* what() const noexcept override;
protected:
  MessageException(const MessageException& other) = default;
  MessageException& operator=(const MessageException& other) & = default;
  MessageException(MessageException&&) = default;
  MessageException& operator=(MessageException&&) = default;
private:
  std::string m_message;
};

/**
 * @brief Exception thrown when trying to call an operation that is not supported by the object.
 */
class InvalidOperationException : public MessageException
{
public:
  explicit InvalidOperationException(std::string message);
  ~InvalidOperationException() override = default;
  InvalidOperationException(const InvalidOperationException& other) = default;
  InvalidOperationException& operator=(const InvalidOperationException& other) & = default;
  InvalidOperationException(InvalidOperationException&&) = default;
  InvalidOperationException& operator=(InvalidOperationException&&) = default;
};

/**
 * @brief Exception thrown when validation of an object failed.
 */
class ValidationException : public MessageException
{
public:
  explicit ValidationException(std::string message);
  ~ValidationException() override = default;
  ValidationException(const ValidationException& other) = default;
  ValidationException& operator=(const ValidationException& other) & = default;
  ValidationException(ValidationException&&) = default;
  ValidationException& operator=(ValidationException&&) = default;
};

/**
 * @brief Exception thrown when parsing to an object failed.
 */
class ParseException : public MessageException
{
public:
  explicit ParseException(std::string message);
  ~ParseException() override = default;
  ParseException(const ParseException& other) = default;
  ParseException& operator=(const ParseException& other) & = default;
  ParseException(ParseException&&) = default;
  ParseException& operator=(ParseException&&) = default;
};

/**
 * @brief Exception thrown when serializing an object failed.
 */
class SerializeException : public MessageException
{
public:
  explicit SerializeException(std::string message);
  ~SerializeException() override = default;
  SerializeException(const SerializeException& other) = default;
  SerializeException& operator=(const SerializeException& other) & = default;
  SerializeException(SerializeException&&) = default;
  SerializeException& operator=(SerializeException&&) = default;
};

}  // namespace xml

}  // namespace sup

#endif  // SUP_XML_EXCEPTIONS_H_
