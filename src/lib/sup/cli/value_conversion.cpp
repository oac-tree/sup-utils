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

#include "value_conversion.h"

#include "exceptions.h"

#include <cmath>
#include <exception>
#include <string_view>

namespace
{

//! The characters the standard conversions silently skip. Spelled out rather than asked of
//! std::isspace, whose answer depends on the locale the process happens to be in.
constexpr std::string_view kWhitespace{" \t\n\v\f\r"};

//! Checks for the two things the conversions below would otherwise accept silently: an empty text,
//! and leading whitespace. Trailing text is caught by the full consumption check instead.
bool HasUnusableForm(const std::string& text)
{
  return text.empty() || (kWhitespace.find(text.front()) != std::string_view::npos);
}

}  // namespace

namespace sup
{
namespace cli
{

void ThrowInvalidValue(const std::string& context, const std::string& text,
                       const std::string& reason)
{
  throw InvalidValueException("Value '" + text + "' of " + context + " " + reason);
}

std::int64_t ParseSignedInteger(const std::string& context, const std::string& text)
{
  static const std::string reason{"is not an integer"};

  if (HasUnusableForm(text))
  {
    ThrowInvalidValue(context, text, reason);
  }

  std::int64_t result{0};
  std::size_t consumed{0U};
  try
  {
    result = std::stoll(text, &consumed);
  }
  catch (const std::exception&)
  {
    ThrowInvalidValue(context, text, reason);
  }

  if (consumed != text.size())
  {
    ThrowInvalidValue(context, text, reason);
  }

  return result;
}

std::uint64_t ParseUnsignedInteger(const std::string& context, const std::string& text)
{
  static const std::string reason{"is not a non-negative integer"};

  // Checked before the conversion: std::stoull reads a negative number and wraps it around to a
  // large positive one, without a word.
  if (HasUnusableForm(text) || (text.front() == '-'))
  {
    ThrowInvalidValue(context, text, reason);
  }

  std::uint64_t result{0U};
  std::size_t consumed{0U};
  try
  {
    result = std::stoull(text, &consumed);
  }
  catch (const std::exception&)
  {
    ThrowInvalidValue(context, text, reason);
  }

  if (consumed != text.size())
  {
    ThrowInvalidValue(context, text, reason);
  }

  return result;
}

double ParseFloatingPoint(const std::string& context, const std::string& text)
{
  static const std::string reason{"is not a number"};

  if (HasUnusableForm(text))
  {
    ThrowInvalidValue(context, text, reason);
  }

  double result{0.0};
  std::size_t consumed{0U};
  try
  {
    result = std::stod(text, &consumed);
  }
  catch (const std::exception&)
  {
    ThrowInvalidValue(context, text, reason);
  }

  if (consumed != text.size())
  {
    ThrowInvalidValue(context, text, reason);
  }

  return result;
}

bool ParseBoolean(const std::string& context, const std::string& text)
{
  if ((text == "1") || (text == "true"))
  {
    return true;
  }
  if ((text == "0") || (text == "false"))
  {
    return false;
  }

  ThrowInvalidValue(context, text, "is not one of '0', '1', 'false' or 'true'");
}

}  // namespace cli

}  // namespace sup
