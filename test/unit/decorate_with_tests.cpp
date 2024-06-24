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

const uint32_t SUCCESS = 1;

// Interface used in the tests.
class CfgItf
{
public:
  virtual ~CfgItf(){};
  virtual uint32_t Method1(const std::string& name, bool& dynamic) const = 0;
  virtual uint32_t Method2(const std::string& name, uint32_t& value) const = 0;
  virtual uint32_t Method3(std::vector<std::string>& dataset_names) const = 0;
  virtual uint32_t Method4(const std::string& name, uint32_t& value) const = 0;
  virtual uint32_t Method5(const std::string& name, const uint32_t& value) = 0;
};

class MockDecorator : public CfgItf
{
public:
  MockDecorator(){};
  MOCK_METHOD(uint32_t, Method1, (const std::string& name, bool& dynamic), (const override));
  MOCK_METHOD(uint32_t, Method2, (const std::string& name, uint32_t& value), (const override));

  MOCK_METHOD(uint32_t, Method3, (std::vector<std::string> & dataset_names), (const override));
  MOCK_METHOD(uint32_t, Method4, (const std::string& name, uint32_t& value), (const override));
  MOCK_METHOD(uint32_t, Method5, (const std::string& name, const uint32_t& value), (override));
};

class FakeDecorator : public CfgItf
{
protected:
  CfgItf& m_configuration;

public:
  FakeDecorator(CfgItf& config) : m_configuration(config){};
  uint32_t Method1(const std::string& name, bool& dynamic) const
  {
    return m_configuration.Method1(name, dynamic);
  }

  uint32_t Method2(const std::string& name, uint32_t& value) const
  {
    return m_configuration.Method2(name, value);
  }

  uint32_t Method3(std::vector<std::string>& dataset_names) const
  {
    return m_configuration.Method3(dataset_names);
  }

  uint32_t Method4(const std::string& name, uint32_t& value) const
  {
    return m_configuration.Method4(name, value);
  }

  uint32_t Method5(const std::string& name, const uint32_t& value)
  {
    return m_configuration.Method5(name, value);
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
      .WillOnce(testing::Return(SUCCESS));

  EXPECT_EQ(mock_decorator->Method1(name, dynamic), SUCCESS);
}

TEST_F(DecorateWithTest, Pass_Mock_Method2_Is_Called)
{
  EXPECT_CALL(*dynamic_cast<MockDecorator*>(mock_decorator.get()), Method2(testing::_, testing::_))
      .WillOnce(testing::Return(SUCCESS));

  EXPECT_EQ(mock_decorator->Method2(name, value), SUCCESS);
}

TEST_F(DecorateWithTest, Pass_Mock_Method3_Is_Called)
{
  EXPECT_CALL(*dynamic_cast<MockDecorator*>(mock_decorator.get()), Method3(testing::_))
      .WillOnce(testing::Return(SUCCESS));

  std::vector<std::string> dataset_names;
  EXPECT_EQ(mock_decorator->Method3(dataset_names), SUCCESS);
}

TEST_F(DecorateWithTest, Pass_Mock_Method4_Is_Called)
{
  EXPECT_CALL(*dynamic_cast<MockDecorator*>(mock_decorator.get()), Method4(testing::_, testing::_))
      .WillOnce(testing::Return(SUCCESS));

  EXPECT_EQ(mock_decorator->Method4(name, value), SUCCESS);
}

TEST_F(DecorateWithTest, Pass_Mock_Method5_Is_Called)
{
  EXPECT_CALL(*dynamic_cast<MockDecorator*>(mock_decorator.get()), Method5(testing::_, testing::_))
      .WillOnce(testing::Return(SUCCESS));

  EXPECT_EQ(mock_decorator->Method5(name, value), SUCCESS);
}

// Fake Decorator
TEST_F(DecorateWithTest, Pass_Fake_Method1_Is_Called)
{
  EXPECT_CALL(*dynamic_cast<MockDecorator*>(mock_decorator.get()), Method1(testing::_, testing::_))
      .WillOnce(testing::Return(SUCCESS));

  std::unique_ptr<CfgItf> fake_decorator = DecorateWith<FakeDecorator>(std::move(mock_decorator));
  EXPECT_EQ(fake_decorator->Method1(name, dynamic), SUCCESS);
}

TEST_F(DecorateWithTest, Pass_Fake_Method2_Is_Called)
{
  EXPECT_CALL(*dynamic_cast<MockDecorator*>(mock_decorator.get()), Method2(testing::_, testing::_))
      .WillOnce(testing::Return(SUCCESS));

  std::unique_ptr<CfgItf> fake_decorator = DecorateWith<FakeDecorator>(std::move(mock_decorator));

  EXPECT_EQ(fake_decorator->Method2(name, value), SUCCESS);
}

TEST_F(DecorateWithTest, Pass_Fake_Method3_Is_Called)
{
  EXPECT_CALL(*dynamic_cast<MockDecorator*>(mock_decorator.get()), Method3(testing::_))
      .WillOnce(testing::Return(SUCCESS));

  std::unique_ptr<CfgItf> fake_decorator = DecorateWith<FakeDecorator>(std::move(mock_decorator));

  std::vector<std::string> dataset_names;
  EXPECT_EQ(fake_decorator->Method3(dataset_names), SUCCESS);
}

TEST_F(DecorateWithTest, Pass_Fake_Method4_Is_Called)
{
  EXPECT_CALL(*dynamic_cast<MockDecorator*>(mock_decorator.get()), Method4(testing::_, testing::_))
      .WillOnce(testing::Return(SUCCESS));

  std::unique_ptr<CfgItf> fake_decorator = DecorateWith<FakeDecorator>(std::move(mock_decorator));

  EXPECT_EQ(fake_decorator->Method4(name, value), SUCCESS);
}

TEST_F(DecorateWithTest, Pass_Fake_Method5_Is_Called)
{
  EXPECT_CALL(*dynamic_cast<MockDecorator*>(mock_decorator.get()), Method5(testing::_, testing::_))
      .WillOnce(testing::Return(SUCCESS));

  std::unique_ptr<CfgItf> fake_decorator = DecorateWith<FakeDecorator>(std::move(mock_decorator));

  EXPECT_EQ(fake_decorator->Method5(name, value), SUCCESS);
}
}  // namespace tests

}  // namespace unit