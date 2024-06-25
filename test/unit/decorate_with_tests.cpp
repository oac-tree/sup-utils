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
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sup/templates/decorate_with.h>

namespace unit
{
namespace tests
{

using namespace sup::templates;

const uint32_t DefaultReturnValue = 5;
const uint32_t Multiplier = 5;
const uint32_t Divisor = 5;
const uint32_t MultiplierResult = DefaultReturnValue*Multiplier;
const uint32_t DivisorResult = DefaultReturnValue/Divisor;

// Interface used in the tests.
class CfgItf
{
public:
  virtual ~CfgItf(){};
  virtual uint32_t Method1(const std::string& name, bool& dynamic) const = 0;
  virtual uint32_t Method2(const std::string& name, uint32_t& value) const = 0;
};

class MockDecorator : public CfgItf
{
public:
  MockDecorator(){};
  MOCK_METHOD(uint32_t, Method1, (const std::string& name, bool& dynamic), (const override));
  MOCK_METHOD(uint32_t, Method2, (const std::string& name, uint32_t& value), (const override));
};

class FakeDecorator : public CfgItf
{
protected:
  CfgItf& m_configuration;

public:
  FakeDecorator(CfgItf& config) : m_configuration(config){};
  uint32_t Method1(const std::string& name, bool& dynamic) const
  {
    return Multiplier*m_configuration.Method1(name, dynamic);
  }

  uint32_t Method2(const std::string& name, uint32_t& value) const
  {
    return static_cast<uint32_t>(m_configuration.Method2(name, value)/Divisor);
  }
};

class DecorateWithTest : public ::testing::Test
{
protected:
  DecorateWithTest() {}
  ~DecorateWithTest() = default;
  std::unique_ptr<CfgItf> mock_decorator{new MockDecorator()};
  std::string name = "test";
  bool dynamic = true;
  uint32_t value;
};

TEST_F(DecorateWithTest, Pass_Mock_Method1_Is_Called)
{
  EXPECT_CALL(*dynamic_cast<MockDecorator*>(mock_decorator.get()), Method1(testing::_, testing::_))
      .WillOnce(testing::Return(DefaultReturnValue));

  EXPECT_EQ(mock_decorator->Method1(name, dynamic), DefaultReturnValue);
}

TEST_F(DecorateWithTest, Pass_Mock_Method2_Is_Called)
{
  EXPECT_CALL(*dynamic_cast<MockDecorator*>(mock_decorator.get()), Method2(testing::_, testing::_))
      .WillOnce(testing::Return(DefaultReturnValue));

  EXPECT_EQ(mock_decorator->Method2(name, value), DefaultReturnValue);
}


// Fake Decorator
TEST_F(DecorateWithTest, Pass_Fake_Method1_Is_Called)
{
  EXPECT_CALL(*dynamic_cast<MockDecorator*>(mock_decorator.get()), Method1(testing::_, testing::_))
      .WillOnce(testing::Return(DefaultReturnValue));

  std::unique_ptr<CfgItf> fake_decorator = DecorateWith<FakeDecorator>(std::move(mock_decorator));
  EXPECT_EQ(fake_decorator->Method1(name, dynamic), MultiplierResult);
}

TEST_F(DecorateWithTest, Pass_Fake_Method2_Is_Called)
{
  EXPECT_CALL(*dynamic_cast<MockDecorator*>(mock_decorator.get()), Method2(testing::_, testing::_))
      .WillOnce(testing::Return(DefaultReturnValue));

  std::unique_ptr<CfgItf> fake_decorator = DecorateWith<FakeDecorator>(std::move(mock_decorator));

  EXPECT_EQ(fake_decorator->Method2(name, value), DivisorResult);
}

}  // namespace tests

}  // namespace unit