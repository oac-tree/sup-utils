==========================
 Base64 Encoding/Decoding
==========================

The ``sup/codec`` package provides utilities for Base64 encoding and decoding.

**Key Features**:
  - Encode binary data to Base64 strings.
  - Decode Base64 strings to binary data.

**Main Components**:
  1. ``Base64Encode``: Encodes a vector of bytes to a Base64 string.
  2. ``Base64Decode``: Decodes a Base64 string to a vector of bytes.

**Example**:

.. code-block:: c++

  std::vector<uint8_t> data = {0x01, 0x02, 0x03};
  std::string encoded = sup::codec::Base64Encode(data);
  std::vector<uint8_t> decoded = sup::codec::Base64Decode(encoded);
