/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : Supervision and Automation System Utilities
 *
 * Description   : SUP utilities
 *
 * Author        : Walter Van Herck (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#ifndef SUP_XML_UNIT_TEST_HELPER_H_
#define SUP_XML_UNIT_TEST_HELPER_H_

#include <chrono>
#include <string>
#include <thread>

namespace sup
{
namespace unit_test_helper
{
class TemporaryTestFile
{
private:
  std::string m_filename;

public:
  TemporaryTestFile(const std::string& filename, const std::string& contents);
  ~TemporaryTestFile();
};

}  // namespace unit_test_helper

}  // namespace sup

#endif  // SUP_XML_UNIT_TEST_HELPER_H_
