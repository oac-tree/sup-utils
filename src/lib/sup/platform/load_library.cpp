/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : Supervision and Automation System Utilities
 *
 * Description   : SUP platform dependent utilities
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

#include <sup/platform/load_library.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace sup
{
namespace platform
{

void* LoadLibrary(const std::string& library_path)
{
#if defined(_WIN32)
  HMODULE handle = ::LoadLibraryA(path.c_str());
  return reinterpret_cast<void*>(handle);
#else
  void* handle = ::dlopen(library_path.c_str(), RTLD_NOW);
  return handle;
#endif
}

bool UnloadLibrary(void* handle)
{
  if (!handle)
  {
    return true;
  }
#if defined(_WIN32)
  return (::FreeLibrary(reinterpret_cast<HMODULE>(handle)) != 0);
#else
  return (::dlclose(handle) == 0);
#endif
}

}  // namespace platform

}  // namespace sup
