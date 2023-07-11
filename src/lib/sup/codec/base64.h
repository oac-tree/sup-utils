/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : Supervision and Automation System Utilities
 *
 * Description   : Command line argument parsing
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

#ifndef SUP_CODEC_BASE64_H_
#define SUP_CODEC_BASE64_H_

#include <sup/codec/base_types.h>

#include <string>
#include <vector>

namespace sup
{
namespace codec
{

std::string Base64Encode(const std::vector<uint8>& data);

std::vector<uint8> Base64Decode(const std::string& str);

}  // namespace codec

}  // namespace sup

#endif  // SUP_CODEC_BASE64_H_
