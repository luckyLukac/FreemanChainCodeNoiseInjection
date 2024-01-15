#pragma once

#include "ChainCode.hpp"
#include "Pixel.hpp"


/// <summary>
/// Type of noise analysis.
/// </summary>
enum class NoiseAnalysisType {
    manhattan,
    euclidean
};



class NoiseAnalyzer {
private:
    std::vector<ChainCode> m_OriginalChainCodes;  // Original non-noisy chain codes.


    /// <summary>
    /// Calculation of the Manhattan distance between two pixels
    /// </summary>
    /// <param name="p1">: first pixel</param>
    /// <param name="p2">: second pixel</param>
    /// <returns>Manhattan distance</returns>
    double manhattanDistance(const Pixel& p1, const Pixel& p2) const;

    /// <summary>
    /// Calculation of the Euclidean distance between two pixels
    /// </summary>
    /// <param name="p1">: first pixel</param>
    /// <param name="p2">: second pixel</param>
    /// <returns>Euclidean distance</returns>
    double euclideanDistance(const Pixel& p1, const Pixel& p2) const;

    /// <summary>
    /// Rasterization of a chain code.
    /// </summary>
    /// <param name="chainCode"></param>
    /// <returns></returns>
    std::vector<Pixel> rasterizeChainCode(const ChainCode& chainCode) const;

    /// <summary>
    /// Noise analysis in the chain code.
    /// </summary>
    /// <param name="chainCode">: chosen chain code</param>
    /// <param name="originalChainCode">: non-noisy chain code</param>
    /// <param name="type">: noise analysis type</param>
    /// <returns>Average value of distances from noisy pixels to original pixels</returns>
    double analyzeNoiseInChainCode(const ChainCode& chainCode, const ChainCode& originalChainCode, NoiseAnalysisType type) const;

public:
    /// <summary>
    /// Main constructor of NoiseAnalyzer.
    /// </summary>
    /// <param name="chainCodes">: vector of chain codes</param>
    NoiseAnalyzer(const std::vector<ChainCode>& chainCodes);

    /// <summary>
    /// Analysis of the present noise in the chain code.
    /// </summary>
    /// <param name="chainCodes">: vector of chain codes</param>
    /// <param name="type">: type of chain codes (F4 or F8)</param>
    /// <returns>Average value of distances from noisy pixels to original pixels, averaged by all chain codes</returns>
    double analyzeNoise(const std::vector<ChainCode>& chainCodes, NoiseAnalysisType type) const;

    /// <summary>
    /// Calculation of the fractal dimension of the object.
    /// </summary>
    /// <param name="chainCodes">: vector of chain codes</param>
    /// <returns>Fractal dimension</returns>
    double fractalDimension(const std::vector<ChainCode>& chainCodes) const;
};