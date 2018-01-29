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

#include "dnsmasq_config.h"

#include <fstream>
#include <iostream>
#include <string_view>
#include <type_traits>

namespace asus {

DnsMasqConfig::ValueType DnsMasqConfig::ParseConfigValue(
    std::string_view key, std::string_view value) {
  if (key == "dhcp-host") {
    auto mac_end = value.find(',');
    // This config line seems invalid.
    if (mac_end == std::string::npos) return std::string{value};

    auto host_end = value.find(',', mac_end + 1);
    // This config already uses host name.
    if (host_end != std::string::npos) return std::string{value};

    return HostInfo{std::string(value.substr(0, mac_end)), std::nullopt,
                    std::string(value.substr(mac_end + 1))};
  }
  return std::string{value};
}

void DnsMasqConfig::Load(const std::string& source) {
  std::ifstream cfg(source);
  if (!cfg.good()) return;

  std::string line;
  while (std::getline(cfg, line)) {
    std::string_view lview(line);
    lview = lview.substr(0, lview.find('#'));
    if (!lview.size()) continue;

    auto epos = lview.find('=');
    if (epos == std::string::npos) {
      options_.emplace(lview, nullptr);
    } else {
      auto key = lview.substr(0, epos);
      auto val = lview.substr(epos + 1);
      options_.emplace(key, ParseConfigValue(key, val));
    }
  }
}

void DnsMasqConfig::RewriteHosts(const HostInfoMap& hosts) {
  for (auto& opt : options_) {
    std::clog << opt.first << " is ";
    std::visit(
        [&hosts](auto&& arg) {
          using T = std::decay_t<decltype(arg)>;
          if
            constexpr(std::is_same_v<T, nullptr_t>) { std::clog << "set"; }
          else if
            constexpr(std::is_same_v<T, std::string>) {
              std::clog << "'" << arg << "'";
            }
          else if
            constexpr(std::is_same_v<T, HostInfo>) {
              auto other_host = hosts.find(arg.Id());
              if (other_host != hosts.end()) {
                arg.MergeFrom(other_host->second);
              }
              std::clog << arg;
            }
          else {
            std::clog << "of unknown type '" << typeid(T).name() << "'";
          }
        },
        opt.second);
    std::clog << '\n';
  }
}

void DnsMasqConfig::Save(const std::string& dest) {
  std::ofstream cfg(dest);
  if (!cfg.good()) {
    std::clog << "Could not save dnsmasq config file " << dest << '\n';
    return;
  }

  cfg << "#\n";
  cfg << "# Config file generated using dnsmasq surrogate\n";
  cfg << "#\n";

  for (auto& opt : options_) {
    cfg << opt.first;
    std::visit(
        [&cfg](auto&& arg) {
          using T = std::decay_t<decltype(arg)>;
          if
            constexpr(std::is_same_v<T, nullptr_t>) {
              // ignore.
            }
          else if
            constexpr(std::is_same_v<T, std::string>) { cfg << '=' << arg; }
          else if
            constexpr(std::is_same_v<T, HostInfo>) {
              cfg << '=' << arg.MacAddr();
              if (arg.Name()) cfg << ',' << arg.Name().value();
              cfg << ',' << arg.IpAddr().value();
            }
          else {
            // Best effort.
            cfg << arg;
          }
        },
        opt.second);
    cfg << '\n';
  }
}

}  // namespace asus