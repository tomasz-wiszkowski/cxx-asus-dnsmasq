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

#include <cstdint>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>

namespace asus {

// HostInfoState describes applicability of a HostInfo structure for DNS
// purposes.
enum class HostInfoState {
  // No problems found.
  OK,

  // MAC address is not valid / could not be processed.
  InvalidMacAddress,

  // Name assigned to host is not valid for DNS.
  UnacceptableName,
};


// HostInfo contains configuration details for a single host.
class HostInfo {
 public:
  HostInfo(
      std::string mac,
      std::optional<std::string> name,
           std::optional<std::string> ip)
      : mac_addr_(std::move(mac)),
        name_(std::move(name)),
        ip_addr_(std::move(ip)) {
    HostMacAddressToId();
    if (state_ == HostInfoState::OK) {
      Validate();
    }
  }

  // Default move constructors
  HostInfo(HostInfo&&) = default;
  HostInfo& operator= (HostInfo&&) = default;
  // Disallow copying of this object.
  HostInfo(const HostInfo&) = delete;
  HostInfo& operator= (const HostInfo&) = delete;

  // Accessors.
  uint64_t Id() const { return id_; }
  const std::string& MacAddr() const { return mac_addr_; }
  const std::optional<std::string>& Name() const { return name_; }
  const std::optional<std::string>& IpAddr() const { return ip_addr_; }

  // MergeFrom collects additional details from a different instance of HostInfo
  // that is built for the same host.
  // Host matching is done using MAC addresses (converted to Host's ID).
  // Host name and ip addresses are populated only if the supplied Host
  // structure reports no problems with its own data.
  //
  // Returns: false, if merge was not successful.
  bool MergeFrom(const HostInfo& other) {
    if (other.id_ != id_) return false;
    if (other.state_ != HostInfoState::OK) return false;

    if (!name_ && other.name_) name_ = other.name_.value();
    if (!ip_addr_ && other.ip_addr_) ip_addr_ = other.ip_addr_.value();

    Validate();
    return true;
  }

 private:
  // Convert MAC address to an ID.
  // This call will erase any previously existing state_ settings.
  void HostMacAddressToId();

  // Validate host name applicability for DNS.
  // This call will erase any previously existing state_ settings.
  void Validate();

  HostInfoState state_ = HostInfoState::OK;

  // id holds MAC address as a 64bit integer.
  uint64_t id_ = ~0ull;

  // MAC address in a regular 6-octet string representation.
  std::string mac_addr_;

  // Host name.
  std::optional<std::string> name_;

  // IP address statically assigned to host.
  std::optional<std::string> ip_addr_;

  // Allow printing details of this class to output stream.
  friend std::ostream& operator<<(std::ostream&, const HostInfo&);
};

using HostInfoMap = std::unordered_map<uint64_t, HostInfo>;

// Logging.
std::ostream& operator<<(std::ostream& out, const HostInfo& ci);
std::ostream& operator<<(std::ostream& out, HostInfoState e);

}  // namespace asus
