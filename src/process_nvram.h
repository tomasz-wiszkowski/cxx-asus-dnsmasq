//
// Copyright 2018 Tomasz Wiszkowski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#pragma once

#include <string>
#include <string_view>
#include <variant>

#include "host_info.h"

namespace asus {
// NVRAM string parsing result.
enum class ParseResult {
  // No error.
  OK,

  // Missing start marker ('<')
  InvalidStartMarker,

  // Missing end marker ('>>')
  InvalidEndMarker,

  // Missing field marker ('>')
  MissingFieldEndMarker,

  // Invalid entry or other processing error.
  InvalidDefinition,
};

// Convert Asus nvram custom_clientlist to a HostInfoMap.
HostInfoMap ProcessCustomClientList(std::string_view input);

// Extract client information from client string.
std::variant<HostInfo, ParseResult> ExtractClientInfo(std::string_view client);

// Logging.
std::ostream& operator<<(std::ostream& out, ParseResult e);

}  // namespace asus
