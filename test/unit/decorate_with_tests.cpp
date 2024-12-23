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
const uint32_t MultiplicationResult = DefaultReturnValue * Multiplier;
const uint32_t DivisionResult = DefaultReturnValue / Divisor;

// Interface used in the tests.
class TestInterface
{
public:
  virtual ~TestInterface(){};
  virtual uint32_t GetMultiplication(const std::string& name, uint32_t value) const = 0;
  virtual uint32_t GetDivision(const std::string& name, uint32_t value) const = 0;
};

class MockInterface : public TestInterface
{
public:
  MockInterface() = default;
  virtual ~MockInterface() { Die(); }
  MOCK_METHOD(uint32_t, GetMultiplication, (const std::string& name, uint32_t value),
              (const override));
  MOCK_METHOD(uint32_t, GetDivision, (const std::string& name, uint32_t value), (const override));
  MOCK_METHOD0(Die, void());
};

class TestDecorator : public TestInterface
{
protected:
  TestInterface& m_configuration;

public:
  TestDecorator(TestInterface& config) : m_configuration(config){};
  ~TestDecorator() { Die(); }
  MOCK_METHOD0(Die, void());
  uint32_t GetMultiplication(const std::string& name, uint32_t value) const
  {
    return Multiplier * m_configuration.GetMultiplication(name, value);
  }

  uint32_t GetDivision(const std::string& name, uint32_t value) const
  {
    return static_cast<uint32_t>(m_configuration.GetDivision(name, value) / Divisor);
  }
};

class DecorateWithTest : public ::testing::Test
{
protected:
  DecorateWithTest() {}
  ~DecorateWithTest() = default;
  std::unique_ptr<TestInterface> mock_interface = std::make_unique<MockInterface>();
  std::string name{"test"};
  uint32_t value{1};
};

using ::testing::InSequence;

TEST_F(DecorateWithTest, Pass_Test_Decorator_Method1_Is_Called)
{
  EXPECT_CALL(*dynamic_cast<MockInterface*>(mock_interface.get()),
              GetMultiplication(testing::_, testing::_))
      .WillOnce(testing::Return(DefaultReturnValue));

  // Mock destructor should be called
  EXPECT_CALL(*dynamic_cast<MockInterface*>(mock_interface.get()), Die());

  std::unique_ptr<TestInterface> test_decorator =
      DecorateWith<TestDecorator>(std::move(mock_interface));

  // Test Decorator destructor should be called
  EXPECT_CALL(*dynamic_cast<TestDecorator*>(test_decorator.get()), Die());

  EXPECT_EQ(test_decorator->GetMultiplication(name, value), MultiplicationResult);
}

TEST_F(DecorateWithTest, Pass_Test_Decorator_Method2_Is_Called)
{
  EXPECT_CALL(*dynamic_cast<MockInterface*>(mock_interface.get()),
              GetDivision(testing::_, testing::_))
      .WillOnce(testing::Return(DefaultReturnValue));

  // Mock destructor should be called
  EXPECT_CALL(*dynamic_cast<MockInterface*>(mock_interface.get()), Die());

  std::unique_ptr<TestInterface> test_decorator =
      DecorateWith<TestDecorator>(std::move(mock_interface));

  // Test_Decorator destructor should be called
  EXPECT_CALL(*dynamic_cast<TestDecorator*>(test_decorator.get()), Die());

  EXPECT_EQ(test_decorator->GetDivision(name, value), DivisionResult);
}

TEST_F(DecorateWithTest, Pass_Mock_And_Test_Decorator_Destructors_Are_Called)
{
  // Mock destructor should be called (added to prevent GMOCK WARNING: Uninteresting mock function
  // call - returning directly).
  EXPECT_CALL(*dynamic_cast<MockInterface*>(mock_interface.get()), Die());

  {
    InSequence seq;
    std::unique_ptr<TestInterface> _mock_interface = std::make_unique<MockInterface>();
    auto mock_p = _mock_interface.get();
    std::unique_ptr<TestInterface> _test_decorator =
        DecorateWith<TestDecorator>(std::move(_mock_interface));

    // Decorator's base destructor body should be called after its direct member destructor:
    EXPECT_CALL(*dynamic_cast<MockInterface*>(mock_p), Die());
    EXPECT_CALL(*dynamic_cast<TestDecorator*>(_test_decorator.get()), Die());
  }
}

}  // namespace tests

}  // namespace unit