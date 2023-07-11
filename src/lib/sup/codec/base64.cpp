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

#include "base64.h"

#include <sup/codec/modp_b64/modp_b64.h>

#include <stdexcept>

namespace
{
using sup::codec::uint8;
const char* GetCharBuffer(const std::vector<uint8>& data)
{
  return reinterpret_cast<const char*>(data.data());
}
}  // unnamed namespace

namespace sup
{
namespace codec
{
static_assert(sizeof(uint8) == 1, "uint8 type needs to have size 1 byte");
static_assert(sizeof(uint32) == 4, "uint32 type needs to have size 4 bytes");

std::string Base64Encode(const std::vector<uint8>& data)
{
  std::string x(modp_b64_encode_len(data.size()), '\0');
  int d = modp_b64_encode(const_cast<char*>(x.data()), GetCharBuffer(data), data.size());
  if (d == -1)
  {
    throw std::runtime_error("sup::codec::Base64Encode(): failure to encode");
  }
  return x;
}


}  // namespace codec

}  // namespace sup
