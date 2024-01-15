#pragma once

#include <chrono>
#include <unordered_set>
#include <random>

#include "ChainCode.hpp"
#include "ChainCodeReplacementLUT.hpp"
#include "Constants.hpp"



class ChainCodeNoise {
private:
    std::vector<ChainCode> m_OriginalChainCodes;
    std::mt19937 m_Generator;
    std::uniform_real_distribution<> m_Random;
    ChainCodeReplacementLUT m_LUT;


    /// <summary>
    /// Adding noise to a chain code.
    /// </summary>
    /// <param name="chainCode">: the given chain code</param>
    /// <param name="startPixel">: first pixel</param>
    /// <param name="borderPixels">: hash table of border pixels</param>
    /// <param name="noiseProbability">: probability of the noise</param>
    /// <param name="numberOfIterations">: number of algorithm iterations</param>
    /// <returns>Noisy chain code</returns>
    ChainCode addNoiseToChainCode(const ChainCode& chainCode, const Pixel& startPixel, std::unordered_set<Pixel>& borderPixels, const double noiseProbability = 0.02, const uint numberOfIterations = 1);

    /// <summary>
    /// Transforming chain code into a sequence of pixels.
    /// </summary>
    /// <param name="startPixel">: pixel at the start</param>
    /// <param name="sequence">: chain code segment directions</param>
    /// <returns>Vector of pixels</returns>
    std::vector<Pixel> chainCodeSegmentToPixels(const ChainCodeType& type, const Pixel& startPixel, const std::vector<short>& sequence);

    /// <summary>
    /// Check whether noise would cause self-touching areas within the given vicinity in the chain code.
    /// </summary>
    /// <param name="type">: type of the chain code (F4 or F8)</param>
    /// <param name="startPixel">: starting pixel</param>
    /// <param name="noiseSequence">: noise directional sequence</param>
    /// <param name="borderPixels">: pixels that lie on the shape border</param>
    /// <param name="excludedPixels">: pixels that are not included in the check</param>
    /// <param name="vicinity">: vicinity of the check</param>
    /// <returns>True if self-touching area occurs, false otherwise</returns>
    bool wouldNoiseCauseSelfTouchingArea(const ChainCodeType& type, const Pixel& startPixel, const std::vector<short>& noiseSequence, const std::unordered_set<Pixel>& borderPixels, const std::vector<Pixel>& excludedPixels, const int vicinity = 1);

    /// <summary>
    /// Saving the chain code image to a JPG file.
    /// </summary>
    /// <param name="chainCodes">: vector of chain codes</param>
    /// <param name="iteration">: index of the current iteration</param>
    /// <param name="name">: name of the chain code group</param>
    /// <param name="probability">: probability of the mutation</param>
    void saveChainCodeImage(const std::vector<ChainCode>& chainCodes, const uint iteration, const std::string name, const uint probability);

    void analyzeNoise(const std::vector<ChainCode>& noisyChainCodes, const uint iteration, const std::string& name, const uint probability) const;

public:
    /// <summary>
    /// Basic constructor of ChainCodeNoise.
    /// </summary>
    ChainCodeNoise() = default;

    /// <summary>
    /// Constructor of ChainCodeNoise.
    /// </summary>
    /// <param name="originalChainCodes">: original chain codes</param>
    ChainCodeNoise(const std::vector<ChainCode>& originalChainCodes);

    /// <summary>
    /// Assignment operator of ChainCodeNoise.
    /// </summary>
    /// <param name="chainCodeNoise">: basic object</param>
    /// <returns>Copied object</returns>
    ChainCodeNoise& operator=(const ChainCodeNoise& chainCodeNoise);

    /// <summary>
    /// Method for noise application to a vector of chain codes.
    /// </summary>
    /// <param name="chainCodes">: given chain codes</param>
    /// <param name="startPixels">: starting pixels of each given chain code</param>
    /// <param name="borderPixels">: hash table of border pixels</param>
    /// <param name="noiseProbability">: probability of the noise</param>
    /// <param name="numberOfIterations">: number of algorithm iterations</param>
    /// <param name="name">: name of chain code group</param>
    /// <returns>Vector of noisified chain codes</returns>
    std::vector<ChainCode> applyNoise(const std::vector<ChainCode>& chainCodes, const std::vector<Pixel>& startPixels, std::unordered_set<Pixel>& borderPixels, const double noiseProbability = 0.02, const uint numberOfIterations = 1, const std::string& name = "Name");
};