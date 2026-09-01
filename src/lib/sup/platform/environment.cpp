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

#include <sup/platform/environment.h>

#include <cstdlib>
#include <string>

namespace
{
// Substring Valgrind places in LD_PRELOAD of every process it instruments.
const std::string kValgrindPreloadMarker = "vgpreload";

// Factor by which test deadlines are multiplied when running under Valgrind.
const double kValgrindTimeoutFactor = 4.0;

}  // unnamed namespace

namespace sup
{
namespace platform
{

bool IsOnCodac()
{
  return std::getenv("CODAC_ROOT") != nullptr;
}

bool IsOnValgrind()
{
  const char* preloaded = std::getenv("LD_PRELOAD");
  return (preloaded != nullptr)
         && (std::string(preloaded).find(kValgrindPreloadMarker) != std::string::npos);
}

double ScaleTimeout(double timeout_sec)
{
  return IsOnValgrind() ? (timeout_sec * kValgrindTimeoutFactor) : timeout_sec;
}

}  // namespace platform

}  // namespace sup
