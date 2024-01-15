#pragma once

#include <unordered_map>

#include "ChainCode.hpp"
#include "Constants.hpp"


class ChainCodeReplacementLUT {
private:
    // Lookup table for chain code noise.
    const ChainCodeLUT m_F8_LUT1 = {
        {{1, 7},    {1, 0},    {2, 0},    {1, 4},     {},    {7, 4}, {6, 0},    {7, 0}},
        {{0, 1}, {0, 1, 2},    {2, 1},    {2, 2}, {0, 3},        {}, {2, 7},    {0, 0}},
        {{0, 2},    {1, 2},    {3, 1},    {3, 2}, {4, 2},    {3, 6},     {},    {1, 6}},
        {{4, 1},    {2, 2},    {2, 3}, {2, 3, 4}, {4, 3},    {4, 4}, {2, 5},        {}},
        {    {},    {3, 0},    {2, 4},    {3, 4}, {3, 5},    {5, 4}, {6, 4},    {5, 0}},
        {{4, 7},        {},    {6, 3},    {4, 4}, {4, 5}, {4, 5, 6}, {6, 5},    {6, 6}},
        {{0, 6},    {7, 2},        {},    {5, 2}, {4, 6},    {5, 6}, {5, 7},    {7, 6}},
        {{0, 7},    {0, 0},    {6, 1},        {}, {0, 5},    {6, 6}, {6, 7}, {6, 7, 0}},
    };

    // Lookup table 2 for chain code noise.
    const ChainCodeLUT m_F8_LUT2 = {
        {{7, 1},    {1, 0},    {2, 0},    {1, 4},     {},    {7, 4}, {6, 0},    {7, 0}},
        {{0, 1}, {2, 1, 0},    {2, 1},    {2, 2}, {0, 3},        {}, {2, 7},    {0, 0}},
        {{0, 2},    {1, 2},    {1, 3},    {3, 2}, {4, 2},    {3, 6},     {},    {1, 6}},
        {{4, 1},    {2, 2},    {2, 3}, {4, 3, 2}, {4, 3},    {4, 4}, {2, 5},        {}},
        {    {},    {3, 0},    {2, 4},    {3, 4}, {5, 3},    {5, 4}, {6, 4},    {5, 0}},
        {{4, 7},        {},    {6, 3},    {4, 4}, {4, 5}, {6, 5, 4}, {6, 5},    {6, 6}},
        {{0, 6},    {7, 2},        {},    {5, 2}, {4, 6},    {5, 6}, {7, 5},    {7, 6}},
        {{0, 7},    {0, 0},    {6, 1},        {}, {0, 5},    {6, 6}, {6, 7}, {0, 7, 6}},
    };

    // Lookup table for F4 chain code noise.
    const ChainCodeLUT m_F4_LUT1 = {
        {{1, 0, 0, 3},       {1, 0},           {},       {3, 0}},
        {      {0, 1}, {0, 1, 1, 2},       {2, 1},           {}},
        {          {},       {1, 2}, {1, 2, 2, 3},       {3, 2}},
        {      {0, 3},           {},       {2, 3}, {0, 3, 3, 2}},
    };

    // Lookup table 2 for F4 chain code noise.
    const ChainCodeLUT m_F4_LUT2 = {
        {{3, 0, 0, 1},       {1, 0},           {},       {3, 0}},
        {      {0, 1}, {2, 1, 1, 0},       {2, 1},           {}},
        {          {},       {1, 2}, {3, 2, 2, 1},       {3, 2}},
        {      {0, 3},           {},       {2, 3}, {2, 3, 3, 0}},
    };

    std::unordered_map<ChainCodeType, std::vector<ChainCodeLUT>> m_Tables;

public:
    /// <summary>
    /// Constructor of the lookup tables.
    /// </summary>
    ChainCodeReplacementLUT();

    /// <summary>
    /// Finding a replacement sequence based by chain code type and the input sequence.
    /// </summary>
    /// <param name="type">: chain code type (F4, F8, VCC...)</param>
    /// <param name="firstTable">: choosing replacement from first table if true</param>
    /// <param name="connectionIn">: pixel inward chain code direction</param>
    /// <param name="connectionOut">: pixel outward chain code direction</param>
    /// <returns>Replacement sequence of chain code orders.</returns>
    std::vector<short> findReplacement(const ChainCodeType& type, const bool firstTable, const short connectionIn, const short connectionOut);
};

