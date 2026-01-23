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

}  // unnamed namespace

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

std::string GetDynamicLibPrefix()
{
#if defined(__linux__)
  return kLinuxLibPrefix;
#elif defined(__APPLE__) && defined(__MACH__)
  return kMacOSLibPrefix;
#elif defined(_WIN32) || defined(__CYGWIN__)
  return kWindowsLibPrefix;
#else
  return "";
#endif
}

std::string GetDynamicLibPostfix()
{
#if defined(__linux__)
  return kLinuxLibPostfix;
#elif defined(__APPLE__) && defined(__MACH__)
  return kMacOSLibPostfix;
#elif defined(_WIN32) || defined(__CYGWIN__)
  return kWindowsLibPostfix;
#else
  return "";
#endif
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

std::pair<std::string, std::string> SplitDynamicLibFilename(const std::string& filename)
{
  const auto separator = '/';
  const auto pos = filename.rfind(separator);
  std::string path{};
  if (pos != std::string::npos)
  {
    path = filename.substr(0, pos + 1);
  }
  const auto stripped_basename = StripDynamicLibName(filename.substr(pos + 1));
  return { path, stripped_basename };
}

std::string CreateFullDynamicLibPath(const std::string& path, const std::string& stripped_basename)
{
  const auto separator = '/';
  auto tmp_path = path;
  if (!tmp_path.empty() && (tmp_path[tmp_path.size() -1] != separator))
  {
    tmp_path.push_back(separator);
  }
  const auto full_basename = CreateDynamicLibName(stripped_basename);
  return tmp_path + full_basename;
}

}  // namespace platform

}  // namespace sup
