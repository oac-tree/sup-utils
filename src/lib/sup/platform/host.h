/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : Supervision and Automation System Utilities
 *
 * Description   : SUP templates
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

#ifndef SUP_PLATFORM_HOST_H_
#define SUP_PLATFORM_HOST_H_

namespace sup
{
namespace platform
{

/**
 * @brief Checks if the current OS is a Linux system.
 */
bool IsLinuxHost();

/**
 * @brief Checks if the current OS is a Windows system.
 */
bool IsWindowsHost();

/**
 * @brief Checks if the current OS is a Mac system.
 */
bool IsMacHost();


}  // namespace platform

}  // namespace sup

#endif  // SUP_PLATFORM_HOST_H_
