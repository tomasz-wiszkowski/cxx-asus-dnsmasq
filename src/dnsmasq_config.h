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

#include <map>
#include <string>
#include <variant>

#include "host_info.h"

namespace asus {
// DNSMasq configuration file handling.
//
// This file processes and re-writes dnsmasq configuration.
class DnsMasqConfig {
 public:
  DnsMasqConfig() = default;

  // Move constructors.
  DnsMasqConfig(DnsMasqConfig&& other) = default;
  DnsMasqConfig& operator=(DnsMasqConfig&& other) = default;

  // Disallow copying of this object.
  DnsMasqConfig(const DnsMasqConfig&) = delete;
  DnsMasqConfig& operator=(const DnsMasqConfig&) = delete;

  // Load dnsmasq configuration file from specified source.
  // Configuration will be processed and storred internally.
  // Original configuration options order is ignored.
  void Load(std::istream& cfg);

  // Rewrite hosts found in previously loaded configuration file to include
  // information found in the supplied hosts map.
  // Only hosts matching required criteria will be merged in map; for more
  // details see HostInfo::MergeFrom.
  void RewriteHosts(const HostInfoMap& hosts);

  // Attach specified hosts file to the dnsmasq.conf.
  // If the specified file exists, it will be appended via addn-hosts to
  // dnsmasq.conf file.
  void AddHostsFile(const std::string& hosts_file);

  // Save dnsmasq configuration to a specified file.
  // Configuration options are saved in a modified order; original order of
  // configuration options will not be preserved.
  void Save(std::ostream& cfg);

 private:
  using ValueType = std::variant<nullptr_t, std::string, HostInfo>;

  // Parse value for a specified key and return a more flexible representation
  // of that value, if any is available.
  ValueType ParseConfigValue(std::string_view key, std::string_view val);

  // List of all dnsmasq options.
  std::multimap<std::string, ValueType> options_;
};
}  // namespace asus
