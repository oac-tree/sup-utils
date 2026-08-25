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

#ifndef SUP_CLI_VALUE_CONVERSION_H_
#define SUP_CLI_VALUE_CONVERSION_H_

#include <sup/cli/exceptions.h>

#include <cstdint>
#include <limits>
#include <string>
#include <type_traits>

namespace sup
{
namespace cli
{

/**
 * @brief Reports a value that cannot be used.
 *
 * @param context What the value belongs to, e.g. "option '--size'".
 * @param text The text of the value, as it was given in the command line.
 * @param reason What is wrong with it, e.g. "is not an integer".
 *
 * @throws InvalidValueException always, with a message naming all three.
 */
[[noreturn]] void ThrowInvalidValue(const std::string& context, const std::string& text,
                                    const std::string& reason);

/**
 * @brief Converts the whole text to a signed integer.
 *
 * The text must consist of nothing but the number. A trailing unit ("64k"), a base prefix
 * ("0x10"), leading whitespace or an empty text are errors, rather than a partial reading.
 *
 * @throws InvalidValueException if the text is not exactly an integer, or does not fit in 64 bits.
 */
std::int64_t ParseSignedInteger(const std::string& context, const std::string& text);

/**
 * @brief Converts the whole text to an unsigned integer.
 *
 * A negative number is an error, and not a value that wraps around to a large positive one.
 *
 * @throws InvalidValueException if the text is not exactly a non-negative integer, or does not fit
 * in 64 bits.
 */
std::uint64_t ParseUnsignedInteger(const std::string& context, const std::string& text);

/**
 * @brief Converts the whole text to a floating point number.
 *
 * Accepts everything a number can be written as, including "0.5" and "1e4".
 *
 * @throws InvalidValueException if the text is not exactly a number.
 */
double ParseFloatingPoint(const std::string& context, const std::string& text);

/**
 * @brief Converts the whole text to a boolean.
 *
 * Accepts "0", "1", "false" and "true".
 *
 * @throws InvalidValueException for anything else.
 */
bool ParseBoolean(const std::string& context, const std::string& text);

/**
 * @brief Converts the whole text of a value to the requested type.
 *
 * The conversion either consumes the whole text or throws: a value the requested type cannot hold
 * is reported, and not silently turned into a number nobody asked for.
 *
 * @param context What the value belongs to, used in the message, e.g. "option '--size'".
 * @param text The text of the value, as it was given in the command line.
 * @return The converted value.
 *
 * @throws InvalidValueException if the text does not convert as a whole, or the number it denotes
 * doesn't fit into the requested type.
 */
template <typename T>
T ConvertValue(const std::string& context, const std::string& text)
{
  static_assert(std::is_same_v<T, std::string> || std::is_arithmetic_v<T>,
                "only std::string and the arithmetic types can be converted to");

  if constexpr (std::is_same_v<T, std::string>)
  {
    return text;
  }
  else if constexpr (std::is_same_v<T, bool>)
  {
    return ParseBoolean(context, text);
  }
  else if constexpr (std::is_floating_point_v<T>)
  {
    const auto result = ParseFloatingPoint(context, text);
    if constexpr (!std::is_same_v<T, double>)
    {
      constexpr auto limit = static_cast<double>(std::numeric_limits<T>::max());
      if ((result > limit) || (result < -limit))
      {
        ThrowInvalidValue(context, text, "is outside the range of the requested type");
      }
    }
    return static_cast<T>(result);
  }
  else if constexpr (std::is_signed_v<T>)
  {
    const auto result = ParseSignedInteger(context, text);
    if ((result < static_cast<std::int64_t>(std::numeric_limits<T>::min()))
        || (result > static_cast<std::int64_t>(std::numeric_limits<T>::max())))
    {
      ThrowInvalidValue(context, text, "is outside the range of the requested type");
    }
    return static_cast<T>(result);
  }
  else
  {
    const auto result = ParseUnsignedInteger(context, text);
    if (result > static_cast<std::uint64_t>(std::numeric_limits<T>::max()))
    {
      ThrowInvalidValue(context, text, "is outside the range of the requested type");
    }
    return static_cast<T>(result);
  }
}

}  // namespace cli

}  // namespace sup

#endif  // SUP_CLI_VALUE_CONVERSION_H_
