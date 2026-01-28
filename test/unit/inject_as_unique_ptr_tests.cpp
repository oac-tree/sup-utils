/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : Supervision and Automation System Utilities
 *
 * Description   : SUP templates
 *
 * Author        : Carlos Miguel Silva (EXT)
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

#include <sup/templates/inject_as_unique_ptr.h>

#include <gtest/gtest.h>

namespace unit
{
namespace tests
{

using namespace sup::templates;

// Interface used in the tests.
class Service
{
public:
  Service(uint32_t value) : m_value{value} {}
  ~Service() = default;

  uint32_t GetValue() const { return m_value; }

private:
  uint32_t m_value;
};

class Client
{
public:
  Client(Service& service) : m_service{service} {}
  ~Client() = default;

  uint32_t GetValue() const { return m_service.GetValue(); }

private:
  Service& m_service;
};

class InjectAsUniquePtrTest : public ::testing::Test
{
protected:
  InjectAsUniquePtrTest() {}
  ~InjectAsUniquePtrTest() = default;
};

TEST_F(InjectAsUniquePtrTest, CompareInjection)
{
  {
    // Standard injection by reference
    const uint32_t value{42};
    Service service{value};
    EXPECT_EQ(service.GetValue(), value);
    Client client{service};
    EXPECT_EQ(client.GetValue(), value);
    EXPECT_EQ(service.GetValue(), value);
  }
  {
    // Injection by unique_ptr
    const uint32_t value{42};
    auto service = std::make_unique<Service>(value);
    EXPECT_EQ(service->GetValue(), value);
    auto client = InjectAsUniquePtr<Client>(std::move(service));
    EXPECT_FALSE(service);
    EXPECT_EQ(client->GetValue(), value);
  }
}

}  // namespace tests

}  // namespace unit
