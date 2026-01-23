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

#ifndef SUP_PLATFORM_LIBRARY_NAMES_H_
#define SUP_PLATFORM_LIBRARY_NAMES_H_

#include <string>

namespace sup
{
namespace platform
{


/**
 * @brief Return the platform dependent prefix for dynamic library names.
 * @return Dynamic library name prefix.
 */
std::string GetDynamicLibPrefix();

/**
 * @brief Return the platform dependent postfix for dynamic library names.
 * @return Dynamic library name postfix.
 */
std::string GetDynamicLibPostfix();

/**
 * @brief Strip the given dynamic library name from its platform dependent pre/postfixes.
 *
 * @note When the pre/postfix is already stripped, nothing happens to that part.
 *
 * @param base_lib_name Library name without path.
 * @return Dynamic library name without platform dependent pre/postfixes.
 */
std::string StripDynamicLibName(const std::string& base_lib_name);

/**
 * @brief Create dynamic library name from its stripped name (no pre- or postfix).
 *
 * @note This function assumes pre- and/or postfix is NOT present.
 *
 * @param stripped_lib_name Stripped library name without path.
 * @return Dynamic library name with platform dependent pre/postfixes.
 */
std::string CreateDynamicLibName(const std::string& stripped_lib_name);

}  // namespace platform

}  // namespace sup

#endif  // SUP_PLATFORM_LIBRARY_NAMES_H_
