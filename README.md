# CAN-encode-decode
This repository contains a C++ header file for encoding and decoding CAN (Controller Area Network) bus messages. It provides functions to convert between raw CAN data and physical values, supporting both big-endian and little-endian formats, as well as signed and unsigned values.
## Features
* Encode and decode CAN signals
* Support for big-endian (Motorola) and little-endian (Intel) formats
* Handling of signed and unsigned values
* Conversion between raw CAN data and physical values
* Support for Vector CAN DB file format
* Texas Instruments IQ notation support
## Usage
- Include the header file in your project:

'''cpp
#include "can_encode_decode_inl.h"
'''
- Decode a CAN signal:

'''cpp
uint8_t frame[8] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
float value = decode(frame, 16, 16, true, false, 0.1, 0);
'''

- Encoding a CAN signal: 
'''C++
uint8_t frame[8] = {0};
encode(frame, 123.4, 16, 16, true, false, 0.1, 0);'''
## Parameters
* is_big_endian: Boolean indicating if the format is big-endian (Motorola) or little-endian (Intel).
* is_signed: Boolean indicating if the value is signed or unsigned.
* factor: Scaling factor for the physical value.
* offset: Offset to be added to the scaled value.
* startbit: The starting bit position of the signal in the CAN frame.
* length: The length of the signal in bits.
* float_length: The length of the fractional part for IQ notation.
## Contributing
Contributions are welcome! Please feel free to submit a Pull Request.
## License
This project is open source and available under the MIT License.
