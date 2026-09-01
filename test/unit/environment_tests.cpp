/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : Supervision and Automation System Utilities
 *
 * Description   : Runtime environment detection
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

#include <gtest/gtest.h>

#include <cstdlib>
#include <string>

using namespace sup::platform;

class EnvironmentTests : public ::testing::Test
{
protected:
  // Saves a variable so a test can restore whatever the runner's environment had.
  static std::string Saved(const char* name)
  {
    const char* value = std::getenv(name);
    return (value == nullptr) ? std::string{} : std::string{value};
  }

  static void Restore(const char* name, const std::string& value)
  {
    if (value.empty())
    {
      (void)::unsetenv(name);
    }
    else
    {
      (void)::setenv(name, value.c_str(), 1);
    }
  }
};

//! IsOnCodac follows CODAC_ROOT, both ways.
TEST_F(EnvironmentTests, IsOnCodacFollowsCodacRoot)
{
  const std::string saved = Saved("CODAC_ROOT");

  (void)::unsetenv("CODAC_ROOT");
  EXPECT_FALSE(IsOnCodac());

  (void)::setenv("CODAC_ROOT", "/opt/codac-7.4", 1);
  EXPECT_TRUE(IsOnCodac());

  Restore("CODAC_ROOT", saved);
}

//! IsOnValgrind reports false when no preload library is set.
TEST_F(EnvironmentTests, IsOnValgrindFalseWithoutPreload)
{
  const std::string saved = Saved("LD_PRELOAD");

  (void)::unsetenv("LD_PRELOAD");
  EXPECT_FALSE(IsOnValgrind());

  Restore("LD_PRELOAD", saved);
}

//! IsOnValgrind ignores a preload library that is not Valgrind's.
TEST_F(EnvironmentTests, IsOnValgrindFalseForUnrelatedPreload)
{
  const std::string saved = Saved("LD_PRELOAD");

  (void)::setenv("LD_PRELOAD", "/usr/lib64/libSegFault.so", 1);
  EXPECT_FALSE(IsOnValgrind());

  Restore("LD_PRELOAD", saved);
}

//! IsOnValgrind recognises the preload libraries Valgrind injects.
TEST_F(EnvironmentTests, IsOnValgrindTrueForValgrindPreload)
{
  const std::string saved = Saved("LD_PRELOAD");

  (void)::setenv("LD_PRELOAD",
                 "/usr/libexec/valgrind/vgpreload_core-amd64-linux.so:"
                 "/usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so",
                 1);
  EXPECT_TRUE(IsOnValgrind());

  Restore("LD_PRELOAD", saved);
}

//! ScaleTimeout leaves the deadline alone when not instrumented.
TEST_F(EnvironmentTests, ScaleTimeoutUnchangedWithoutValgrind)
{
  const std::string saved = Saved("LD_PRELOAD");

  (void)::unsetenv("LD_PRELOAD");
  EXPECT_DOUBLE_EQ(ScaleTimeout(1.0), 1.0);
  EXPECT_DOUBLE_EQ(ScaleTimeout(0.0), 0.0);

  Restore("LD_PRELOAD", saved);
}

//! ScaleTimeout widens the deadline under Valgrind.
TEST_F(EnvironmentTests, ScaleTimeoutWidenedUnderValgrind)
{
  const std::string saved = Saved("LD_PRELOAD");

  (void)::setenv("LD_PRELOAD", "/usr/libexec/valgrind/vgpreload_core-amd64-linux.so", 1);
  EXPECT_GT(ScaleTimeout(1.0), 1.0);
  EXPECT_DOUBLE_EQ(ScaleTimeout(0.0), 0.0);

  Restore("LD_PRELOAD", saved);
}
