#pragma once

#include <array>
#include <numeric>
#include <string>
#include <vector>

// CONSTANTS
const double MAX = std::numeric_limits<double>::max();
const double MIN = -std::numeric_limits<double>::max();

// ALIASES
using uint = unsigned int;
using u64 = uint64_t;
using ChainCodeLUT = std::vector<std::vector<std::vector<short>>>;
using PixelField = std::vector<std::vector<bool>>;