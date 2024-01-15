#include <cmath>
#include <exception>

#include "NoiseAnalyzer.hpp"


double NoiseAnalyzer::manhattanDistance(const Pixel& p1, const Pixel& p2) const {
    return std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
}

double NoiseAnalyzer::euclideanDistance(const Pixel& p1, const Pixel& p2) const {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}

std::vector<Pixel> NoiseAnalyzer::rasterizeChainCode(const ChainCode& chainCode) const {
    std::vector<Pixel> pixels;

    // Adding the first pixel to the vector.
    Pixel movingPixel(chainCode.startX, chainCode.startY);
    pixels.push_back(movingPixel);

    // Moving along the chain code and adding pixels.
    for (short order : chainCode.code) {
        movingPixel = ChainCodeFunctions::chainCodeMove(chainCode.type, order, movingPixel);
        pixels.push_back(movingPixel);
    }

    return pixels;
}

double NoiseAnalyzer::analyzeNoiseInChainCode(const ChainCode& chainCode, const ChainCode& originalChainCode, NoiseAnalysisType type) const {
    double distanceSum = 0.0;
    
    // Rasterization of the original and the noisy chain code.
    const std::vector<Pixel> rasterizedOriginalChainCode = rasterizeChainCode(originalChainCode);
    const std::vector<Pixel> rasterizedChainCode = rasterizeChainCode(chainCode);

    for (const Pixel& pixel : rasterizedChainCode) {
        double minDistance = MAX;
        for (const Pixel& originalPixel : rasterizedOriginalChainCode) {
            double currentDistance = 0.0;
            if (type == NoiseAnalysisType::manhattan) {
                currentDistance = manhattanDistance(pixel, originalPixel);
            }
            else if (type == NoiseAnalysisType::euclidean) {
                currentDistance = euclideanDistance(pixel, originalPixel);
            }

            if (currentDistance < minDistance) {
                minDistance = currentDistance;
            }
        }

        distanceSum += minDistance;
    }

    return distanceSum / rasterizedChainCode.size();
}


NoiseAnalyzer::NoiseAnalyzer(const std::vector<ChainCode>& chainCodes) : m_OriginalChainCodes(chainCodes) {}

double NoiseAnalyzer::analyzeNoise(const std::vector<ChainCode>& chainCodes, NoiseAnalysisType type) const {
    double sum = 0.0;
    
    for (uint i = 0; i < chainCodes.size(); i++) {
        const double currentSum = analyzeNoiseInChainCode(chainCodes[i], m_OriginalChainCodes[i], type);
        sum += currentSum;
    }

    return sum / chainCodes.size();
}

double NoiseAnalyzer::fractalDimension(const std::vector<ChainCode>& chainCodes) const {
    if (chainCodes.empty()) {
        throw std::exception("Chain code vector is empty.");
    }
    
    uint occupiedPixels = 0;
    for (const ChainCode& chainCode : chainCodes) {
        occupiedPixels += chainCode.code.size();
    }

    const std::vector<Pixel> rasterizedChainCode = rasterizeChainCode(chainCodes[0]);
    int minX = MAX;
    int maxX = MIN;
    int minY = MAX;
    int maxY = MIN;

    for (const Pixel& pixel : rasterizedChainCode) {
        if (pixel.x < minX) {
            minX = pixel.x;
        }
        if (pixel.x > maxX) {
            maxX = pixel.x;
        }
        if (pixel.y < minY) {
            minY = pixel.y;
        }
        if (pixel.y > maxY) {
            maxY = pixel.y;
        }
    }
    const uint deltaX = maxX - minX;
    const uint deltaY = maxY - minY;
    const double pixelsByLongerSide = std::max(deltaX, deltaY);

    return std::log(occupiedPixels) / std::log(pixelsByLongerSide);
}
