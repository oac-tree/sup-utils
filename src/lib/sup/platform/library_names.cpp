/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : Supervision and Automation System Utilities
 *
 * Description   : SUP platform dependent utilities
 *
 * Author        : Walter Van Herck (IO)
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

#include <sup/platform/library_names.h>
#include <sup/platform/host.h>

namespace sup
{
namespace platform
{
namespace
{
const std::string kLinuxLibPrefix = "lib";
const std::string kLinuxLibPostfix = ".so";

const std::string kMacOSLibPrefix = "lib";
const std::string kMacOSLibPostfix = ".dylib";

const std::string kWindowsLibPrefix = "";
const std::string kWindowsLibPostfix = ".dll";

std::string StripPrefix(const std::string& input, const std::string& prefix);
std::string StripPostfix(const std::string& input, const std::string& postfix);

}  // unnamed namespace

std::string GetDynamicLibPrefix()
{
  if (IsLinuxHost())
  {
    return kLinuxLibPrefix;
  }
  else if (IsMacHost())
  {
    return kMacOSLibPrefix;
  }
  else if (IsWindowsHost())
  {
    return kWindowsLibPrefix;
  }
  return "";
}

std::string GetDynamicLibPostfix()
{
  if (IsLinuxHost())
  {
    return kLinuxLibPostfix;
  }
  else if (IsMacHost())
  {
    return kMacOSLibPostfix;
  }
  else if (IsWindowsHost())
  {
    return kWindowsLibPostfix;
  }
  return "";
}

std::string StripDynamicLibName(const std::string& base_lib_name)
{
  std::string prefix = GetDynamicLibPrefix();
  std::string postfix = GetDynamicLibPostfix();
  auto temp = StripPrefix(base_lib_name, prefix);
  return StripPostfix(temp, postfix);
}

std::string CreateDynamicLibName(const std::string& stripped_lib_name)
{
  return GetDynamicLibPrefix() + stripped_lib_name + GetDynamicLibPostfix();
}

namespace
{
std::string StripPrefix(const std::string& input, const std::string& prefix)
{
  if (prefix.empty() || (prefix.size() > input.size()))
  {
    return input;
  }
  if (input.substr(0, prefix.size()) == prefix)
  {
    return input.substr(prefix.size());
  }
  return input;
}

std::string StripPostfix(const std::string& input, const std::string& postfix)
{
  if (postfix.empty() || (postfix.size() > input.size()))
  {
    return input;
  }
  const auto input_size = input.size();
  const auto postfix_size = postfix.size();
  if (input.substr(input_size - postfix_size, postfix_size) == postfix)
  {
    return input.substr(0, input_size - postfix_size);
  }
  return input;
}

}  // unnamed namespace

}  // namespace platform

}  // namespace sup
