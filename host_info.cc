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
#include "host_info.h"

#include <iomanip>

namespace asus {

void HostInfo::HostMacAddressToId() {
  state_ = HostInfoState::OK;

  union {
    uint64_t mac = 0;
    uint8_t bytes[8];
  } res;

  if (sscanf(mac_addr_.data(), "%2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx",
             &res.bytes[5], &res.bytes[4], &res.bytes[3], &res.bytes[2],
             &res.bytes[1], &res.bytes[0]) != 6) {
    state_ = HostInfoState::InvalidMacAddress;
    return;
  }

  id_ = res.mac;
}

void HostInfo::Validate() {
  state_ = HostInfoState::OK;

  if (!name_) return;

  for (auto c : name_.value()) {
    switch (c) {
      case '0' ... '9':
      case 'a' ... 'z':
      case 'A' ... 'Z':
      case '_':
        break;

      default:
        state_ = HostInfoState::UnacceptableName;
        return;
    }
  }
}

std::ostream& operator<<(std::ostream& out, HostInfoState e) {
  switch (e) {
    case HostInfoState::OK:
      out << "OK";
      break;

    case HostInfoState::InvalidMacAddress:
      out << "InvalidMacAddress";
      break;

    case HostInfoState::UnacceptableName:
      out << "UnacceptableName";
      break;
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, const HostInfo& ci) {
  out << std::setw(12) << std::setfill('0') << std::hex << ci.id_
      << " - Client";
  if (ci.name_) out << " '" << ci.name_.value() << "'";
  out << " (" << ci.mac_addr_ << ")";
  if (ci.ip_addr_) out << " @" << ci.ip_addr_.value();
  out << " [" << ci.state_ << "]";
  return out;
}

}  // namespace asus
