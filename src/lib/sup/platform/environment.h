/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : Supervision and Automation System Utilities
 *
 * Description   : SUP platform dependent utilities
 *
 * Author        : Marcelo Mariano (External)
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

#ifndef SUP_PLATFORM_ENVIRONMENT_H_
#define SUP_PLATFORM_ENVIRONMENT_H_

namespace sup
{
namespace platform
{

/**
 * @brief Checks if the application is running in a CODAC environment.
 *
 * @return true if the CODAC environment variables are set.
 */
bool IsOnCodac();

/**
 * @brief Checks if the application is running under Valgrind.
 *
 * @return true if this process is instrumented; false for one Valgrind was told to skip.
 */
bool IsOnValgrind();

/**
 * @brief Scales a test deadline for the environment this process runs in.
 *
 * @param timeout_sec deadline in seconds, as chosen for a native run.
 * @return the deadline to use, unchanged unless this process is instrumented.
 */
double ScaleTimeout(double timeout_sec);

}  // namespace platform

}  // namespace sup

#endif  // SUP_PLATFORM_ENVIRONMENT_H_
