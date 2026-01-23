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

#include <sup/platform/host.h>

namespace sup
{
namespace platform
{

bool IsLinuxHost()
{
#if defined(__linux__)
  return true;
#else
  return false;
#endif
}

bool IsWindowsHost()
{
#if defined(_WIN32) || defined(__CYGWIN__)
  return true;
#else
  return false;
#endif
}

bool IsMacHost()
{
#if defined(__APPLE__) && defined(__MACH__)
  return true;
#else
  return false;
#endif
}

}  // namespace platform

}  // namespace sup
