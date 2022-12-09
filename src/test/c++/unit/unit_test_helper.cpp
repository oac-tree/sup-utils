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
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "unit_test_helper.h"

#include <fstream>
#include <sstream>

namespace sup
{
namespace unit_test_helper
{

TemporaryTestFile::TemporaryTestFile(std::string filename, std::string contents)
    : m_filename{filename}
{
  std::ofstream file_out(m_filename);
  file_out.write(contents.c_str(), contents.size());
}

TemporaryTestFile::~TemporaryTestFile()
{
  std::remove(m_filename.c_str());
}

std::string GetTextFileContent(const std::string& file_name)
{
  std::ifstream in(file_name);
  std::stringstream sstr;
  while(in >> sstr.rdbuf());
  return sstr.str();
}

}  // namespace unit_test_helper

}  // namespace sup
