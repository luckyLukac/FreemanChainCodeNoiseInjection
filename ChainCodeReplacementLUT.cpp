#include "ChainCodeReplacementLUT.hpp"

ChainCodeReplacementLUT::ChainCodeReplacementLUT() {
    m_Tables[ChainCodeType::F8] = std::vector({ m_F8_LUT1, m_F8_LUT2 });
    m_Tables[ChainCodeType::F4] = std::vector({ m_F4_LUT1, m_F4_LUT2 });
}

std::vector<short> ChainCodeReplacementLUT::findReplacement(const ChainCodeType& type, const bool firstTable, const short connectionIn, const short connectionOut) {
    const short first = firstTable ? 0 : 1;
    return m_Tables[type][first][connectionIn][connectionOut];
}