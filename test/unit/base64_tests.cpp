/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : Supervision and Automation System Utilities
 *
 * Description   : SUP logging
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include <sup/codec/base64.h>

#include <gtest/gtest.h>

#include <random>
#include <stdexcept>

using namespace sup::codec;

// RFC 4648 test vectors:
std::vector<std::pair<std::string, std::string>> kTestVectors{{
  { "f", "Zg==" },
  { "fo", "Zm8=" },
  { "foo", "Zm9v" },
  { "foob", "Zm9vYg==" },
  { "fooba", "Zm9vYmE=" },
  { "foobar", "Zm9vYmFy" },
}};

class Base64Test : public ::testing::Test
{
protected:
  Base64Test();
  virtual ~Base64Test();

  const std::vector<uint8> empty_bytes;
  const std::string empty_str;
  static std::vector<uint8> FromString(const std::string& str);
  static std::vector<uint8> GetRandomBytes(std::size_t size);
};

TEST_F(Base64Test, Empty)
{
  EXPECT_EQ(Base64Encode(empty_bytes), empty_str);
  EXPECT_EQ(Base64Decode(empty_str), empty_bytes);
}

TEST_F(Base64Test, RFC4648TestVectors)
{
  for (const auto& test : kTestVectors)
  {
    std::vector<uint8> bytes = FromString(test.first);
    std::string str = test.second;
    EXPECT_EQ(Base64Encode(bytes), str);
    EXPECT_EQ(Base64Decode(str), bytes);
  }
}

TEST_F(Base64Test, Failure)
{
  std::string contains_whitespace = "this cannot be a base64 string";
  EXPECT_THROW(Base64Decode(contains_whitespace), std::runtime_error);
}

TEST_F(Base64Test, Random)
{
  {
    std::size_t size = 100u;
    auto bytes = GetRandomBytes(size);
    auto str = Base64Encode(bytes);
    EXPECT_EQ(Base64Decode(str), bytes);
  }
  {
    std::size_t size = 101u;
    auto bytes = GetRandomBytes(size);
    auto str = Base64Encode(bytes);
    EXPECT_EQ(Base64Decode(str), bytes);
  }
  {
    std::size_t size = 102u;
    auto bytes = GetRandomBytes(size);
    auto str = Base64Encode(bytes);
    EXPECT_EQ(Base64Decode(str), bytes);
  }
}

TEST_F(Base64Test, RandomLong)
{
  {
    std::size_t size = 1000u;
    auto bytes = GetRandomBytes(size);
    auto str = Base64Encode(bytes);
    EXPECT_EQ(Base64Decode(str), bytes);
  }
  {
    std::size_t size = 10000u;
    auto bytes = GetRandomBytes(size);
    auto str = Base64Encode(bytes);
    EXPECT_EQ(Base64Decode(str), bytes);
  }
  {
    std::size_t size = 100000u;
    auto bytes = GetRandomBytes(size);
    auto str = Base64Encode(bytes);
    EXPECT_EQ(Base64Decode(str), bytes);
  }
}

Base64Test::Base64Test()
  : empty_bytes{}
  , empty_str{}
{}

Base64Test::~Base64Test() = default;

std::vector<uint8> Base64Test::FromString(const std::string& str)
{
  std::vector<uint8> result;
  for (const auto& ch : str)
  {
    result.push_back(ch);
  }
  return result;
}

std::vector<uint8> Base64Test::GetRandomBytes(std::size_t size)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint8> distrib{};

  std::vector<uint8> result(size, 0);
  for (std::size_t i = 0; i < size; ++i)
  {
    result[i] = distrib(gen);
  }
  return result;
}