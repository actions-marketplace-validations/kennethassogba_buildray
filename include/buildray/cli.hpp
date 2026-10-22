#pragma once

#include <string>
#include <vector>

namespace buildray {
struct Options;
struct CheckOptions;
const char* usage(bool advanced = false);
Options parse(const std::vector<std::string>& args);
CheckOptions parse_check(const std::vector<std::string>& args);
}  // namespace buildray
