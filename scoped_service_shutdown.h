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

namespace asus {

// ScopedServiceShutdown ensures service is properly re-activated at the end of
// current execution scope.
// Follows RAII pattern; to shut down a service, simply instantiate the class.
// Service will be restored automatically at the end of scope, eg.
//
//   ...
//   {
//     ScopedServiceShutdown srv("service");
//     // Service is disabled; proceed with applying changes.
//   }
class ScopedServiceShutdown {
 public:
  ScopedServiceShutdown(std::string_view svc) : service_(svc) {
    system(("service stop_" + service_).c_str());
    // Give time to complete execution or cleanup by foreign service that just
    // received a signal from Asus' RC utility.
    usleep(300000);
  }

  ~ScopedServiceShutdown() { system(("service start_" + service_).c_str()); }

 private:
  std::string service_;
};

}  // namespace asus
