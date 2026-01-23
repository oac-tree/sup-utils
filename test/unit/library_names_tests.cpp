/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : Supervision and Automation System Utilities
 *
 * Description   : Command line argument parsing
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
#include <sup/platform/library_names.h>

#include <gtest/gtest.h>

using namespace sup::platform;

class LibraryNamesTests : public ::testing::Test
{
};

TEST_F(LibraryNamesTests, StripDynamicLibName)
{
  const std::string lib_base_name = "MyLibrary";
  {
    // Library name with correct pre- and postfix gets stripped
    const std::string lib_name = GetDynamicLibPrefix() + lib_base_name + GetDynamicLibPostfix();
    EXPECT_EQ(StripDynamicLibName(lib_name), lib_base_name);
  }
  {
    // Library name with only correct prefix gets stripped
    const std::string lib_name = GetDynamicLibPrefix() + lib_base_name;
    EXPECT_EQ(StripDynamicLibName(lib_name), lib_base_name);
  }
  {
    // Library name with only correct postfix gets stripped
    const std::string lib_name = lib_base_name + GetDynamicLibPostfix();
    EXPECT_EQ(StripDynamicLibName(lib_name), lib_base_name);
  }
  {
    // Library name with postfix and version number is not stripped
    const std::string lib_name = lib_base_name + GetDynamicLibPostfix() + ".1";
    EXPECT_EQ(StripDynamicLibName(lib_name), lib_name);
  }
  {
    // Library name with prefix in the middle is not stripped
    const std::string lib_name = "a" + GetDynamicLibPrefix() + lib_base_name;
    EXPECT_EQ(StripDynamicLibName(lib_name), lib_name);
  }
}

TEST_F(LibraryNamesTests, CreateDynamicLibName)
{
  const std::string test{"MyLibrary"};
  EXPECT_EQ(StripDynamicLibName(CreateDynamicLibName(test)), test);
  EXPECT_EQ(CreateDynamicLibName(test), "libMyLibrary.so");
}

TEST_F(LibraryNamesTests, SplitDynamicLibFilename)
{
  const std::string lib_base_name = "MyLibrary";
  {
    // Without path
    const std::string lib_name = GetDynamicLibPrefix() + lib_base_name + GetDynamicLibPostfix();
    const auto [path, stripped_name] = SplitDynamicLibFilename(lib_name);
    EXPECT_TRUE(path.empty());
    EXPECT_EQ(stripped_name, lib_base_name);
  }
  {
    // In root folder with only prefix
    const std::string lib_path = "/";
    const std::string lib_name = lib_path + GetDynamicLibPrefix() + lib_base_name;
    const auto [path, stripped_name] = SplitDynamicLibFilename(lib_name);
    EXPECT_EQ(path, lib_path);
    EXPECT_EQ(stripped_name, lib_base_name);
  }
  {
    // In relative folder with only postfix
    const std::string lib_path = "home/user/";
    const std::string lib_name = lib_path + lib_base_name + GetDynamicLibPostfix();
    const auto [path, stripped_name] = SplitDynamicLibFilename(lib_name);
    EXPECT_EQ(path, lib_path);
    EXPECT_EQ(stripped_name, lib_base_name);
  }
  {
    // In absolute folder with only prefix
    const std::string lib_path = "/home/user/";
    const std::string lib_name = lib_path + GetDynamicLibPrefix() + lib_base_name;
    const auto [path, stripped_name] = SplitDynamicLibFilename(lib_name);
    EXPECT_EQ(path, lib_path);
    EXPECT_EQ(stripped_name, lib_base_name);
  }
}

TEST_F(LibraryNamesTests, CreateFullDynamicLibPath)
{
  const std::string lib_base_name = "MyLibrary";
  const std::string lib_full_name = CreateDynamicLibName(lib_base_name);
  {
    // Without path
    const std::string lib_name = GetDynamicLibPrefix() + lib_base_name + GetDynamicLibPostfix();
    const auto [path, stripped_name] = SplitDynamicLibFilename(lib_name);
    EXPECT_EQ(CreateFullDynamicLibPath(path, stripped_name), lib_name);
  }
  {
    // In root folder with only prefix
    const std::string lib_path = "/";
    const std::string lib_name = lib_path + GetDynamicLibPrefix() + lib_base_name;
    const auto [path, stripped_name] = SplitDynamicLibFilename(lib_name);
    EXPECT_EQ(CreateFullDynamicLibPath(path, stripped_name), lib_path + lib_full_name);
  }
  {
    // In relative folder with only postfix
    const std::string lib_path = "home/user/";
    const std::string lib_name = lib_path + lib_base_name + GetDynamicLibPostfix();
    const auto [path, stripped_name] = SplitDynamicLibFilename(lib_name);
    EXPECT_EQ(CreateFullDynamicLibPath(path, stripped_name), lib_path + lib_full_name);
  }
  {
    // In absolute folder with only prefix
    const std::string lib_path = "/home/user/";
    const std::string lib_name = lib_path + GetDynamicLibPrefix() + lib_base_name;
    const auto [path, stripped_name] = SplitDynamicLibFilename(lib_name);
    EXPECT_EQ(CreateFullDynamicLibPath(path, stripped_name), lib_path + lib_full_name);
  }
}