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

#ifndef SUP_CLI_EXCEPTIONS_H_
#define SUP_CLI_EXCEPTIONS_H_

#include <stdexcept>
#include <string>

namespace sup
{
namespace cli
{

/**
 * @brief Base class of the exceptions thrown by this library.
 *
 * It derives from std::runtime_error, and not from std::exception as the exceptions of some other
 * packages do, because the parser used to report every failure as a plain std::runtime_error.
 * Applications catching that keep working unchanged.
 */
class CommandLineException : public std::runtime_error
{
public:
  explicit CommandLineException(const std::string& message);
  ~CommandLineException() override = default;

protected:
  CommandLineException(const CommandLineException& other) = default;
  CommandLineException& operator=(const CommandLineException& other) & = default;
  CommandLineException(CommandLineException&&) noexcept = default;
  CommandLineException& operator=(CommandLineException&&) & noexcept = default;
};

/**
 * @brief Exception thrown when an option name does not denote any of the registered options.
 *
 * It reports a mistake in the application, not in the command line: the name asked about is not one
 * that was ever added to the parser.
 */
class InvalidOptionException : public CommandLineException
{
public:
  explicit InvalidOptionException(const std::string& message);
  ~InvalidOptionException() override = default;
  InvalidOptionException(const InvalidOptionException& other) = default;
  InvalidOptionException& operator=(const InvalidOptionException& other) & = default;
  InvalidOptionException(InvalidOptionException&&) noexcept = default;
  InvalidOptionException& operator=(InvalidOptionException&&) & noexcept = default;
};

/**
 * @brief Exception thrown when the text of a value is not what the requested type needs.
 *
 * It reports a mistake in the command line: the text does not convert as a whole to the requested
 * type, or the number it denotes falls outside the admissible range.
 */
class InvalidValueException : public CommandLineException
{
public:
  explicit InvalidValueException(const std::string& message);
  ~InvalidValueException() override = default;
  InvalidValueException(const InvalidValueException& other) = default;
  InvalidValueException& operator=(const InvalidValueException& other) & = default;
  InvalidValueException(InvalidValueException&&) noexcept = default;
  InvalidValueException& operator=(InvalidValueException&&) & noexcept = default;
};

/**
 * @brief Exception thrown when no value is available to return.
 *
 * Either the option was not given on the command line and carries no default value, or a
 * positional value was asked for beyond the ones that were found.
 */
class MissingValueException : public CommandLineException
{
public:
  explicit MissingValueException(const std::string& message);
  ~MissingValueException() override = default;
  MissingValueException(const MissingValueException& other) = default;
  MissingValueException& operator=(const MissingValueException& other) & = default;
  MissingValueException(MissingValueException&&) noexcept = default;
  MissingValueException& operator=(MissingValueException&&) & noexcept = default;
};

}  // namespace cli

}  // namespace sup

#endif  // SUP_CLI_EXCEPTIONS_H_
