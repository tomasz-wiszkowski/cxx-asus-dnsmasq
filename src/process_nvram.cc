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
#include "process_nvram.h"

#include <iomanip>
#include <iostream>

namespace asus {
namespace {
// Check if everyhing is in format: <clientname>mac>X>Y>>
ParseResult ValidateClient(std::string_view client) {
  if (client.compare(0, 1, "<")) return ParseResult::InvalidStartMarker;
  if (client.compare(client.size() - 2, 2, ">>"))
    return ParseResult::InvalidEndMarker;
  return ParseResult::OK;
}
}  // namespace

std::variant<HostInfo, ParseResult> ExtractClientInfo(std::string_view client) {
  if (auto val = ValidateClient(client); val != ParseResult::OK) return val;
  client = client.substr(1, client.size() - 3);

  auto name_end = client.find('>', 0);
  if (name_end == std::string::npos) return ParseResult::MissingFieldEndMarker;
  auto mac_end = client.find('>', name_end + 1);
  if (mac_end == std::string::npos) return ParseResult::MissingFieldEndMarker;

  auto res = HostInfo::WithName(
      std::string(client.substr(name_end + 1, mac_end - name_end - 1)),
      std::string(client.substr(0, name_end)));

  if (std::holds_alternative<HostInfo>(res)) {
    return std::get<HostInfo>(std::move(res));
  }
  return ParseResult::InvalidDefinition;
}

HostInfoMap ProcessCustomClientList(std::string_view input) {
  HostInfoMap res;

  // Split large solid string of clients to individual pieces.
  std::string::size_type pos = 0;
  while (auto nextpos = input.find('<', pos + 1)) {
    if (nextpos != std::string::npos) nextpos -= pos;
    std::visit(
        [&res](auto&& v) {
          using T = std::decay_t<decltype(v)>;
          if constexpr (std::is_same_v<T, ParseResult>) {
          } else if constexpr (std::is_same_v<T, HostInfo>) {
            res.emplace(v.Id(), std::move(v));
          }
        },
        ExtractClientInfo(input.substr(pos, nextpos)));

    pos += nextpos;
    if (nextpos == std::string::npos) break;
  }

  return res;
}

std::ostream& operator<<(std::ostream& out, ParseResult e) {
  switch (e) {
    case ParseResult::OK:
      out << "OK";
      break;
    case ParseResult::InvalidStartMarker:
      out << "InvalidStartMarker";
      break;
    case ParseResult::InvalidEndMarker:
      out << "InvalidEndMarker";
      break;
    case ParseResult::MissingFieldEndMarker:
      out << "MissingFieldEndMarker";
      break;
    case ParseResult::InvalidDefinition:
      out << "InvalidDefinition";
      break;
  }
  return out;
}
}  // namespace asus
