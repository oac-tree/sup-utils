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

#ifndef SUP_PLATFORM_LOAD_LIBRARY_H_
#define SUP_PLATFORM_LOAD_LIBRARY_H_

#include <string>

namespace sup
{
namespace platform
{

/**
 * @brief Try to dynamically load the given library.
 *
 * @param library_path Path of the library (may be relative or absolute). Names without `/` will
 * also be looked up in the system's library paths.
 * @return Handle to the library or nullptr when loading failed.
 */
void* LoadLibrary(const std::string& library_path);

/**
 * @brief Try to unload a dynamically loaded library.
 *
 * @param handle Handle to loaded library.
 * @return Boolean indicating success.
 */
bool UnloadLibrary(void* handle);

}  // namespace platform

}  // namespace sup

#endif  // SUP_PLATFORM_LOAD_LIBRARY_H_
