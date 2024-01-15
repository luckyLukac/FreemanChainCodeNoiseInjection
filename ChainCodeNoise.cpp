#include <chrono>
#include <fstream>
#include <iostream>
#include <QPainter>
#include <QImage>
#include <QGraphicsScene>
#include <QDir>
#include <sstream>

#include "ChainCodeNoise.hpp"
#include "NoiseAnalyzer.hpp"


ChainCode ChainCodeNoise::addNoiseToChainCode(const ChainCode& chainCode, const Pixel& startPixel, std::unordered_set<Pixel>& borderPixels, const double noiseProbability, const uint numberOfIterations) {
    ChainCode noisyChainCode = chainCode;

    Pixel previousPixel = startPixel;
    Pixel currentPixel = startPixel;

    for (uint i = 0; i < noisyChainCode.code.size() - 1; i++) {
        // Getting the sequence of chain code orders.
        const short first = noisyChainCode.code[i];
        const short second = noisyChainCode.code[i + 1];

        // Calculation of a random number within the range [0, 1] (noise probability).
        const double randomNumber = m_Random(m_Generator);

        // If a random number is within noise probability range, we manipulate the chain code.
        if (randomNumber < noiseProbability) {
            // Searching the replacement code in the lookup table.
            const bool firstTable = m_Random(m_Generator) < 0.5 ? true : false;
            //const bool firstTable = false;
            const std::vector<short>& replacement = m_LUT.findReplacement(noisyChainCode.type, firstTable, first, second);
                
            // If a combination should not exist, we don't touch shite and move on.
            if (replacement.empty()) {
                continue;
            }
                
            // Creating a vector of excluded pixels in self-touching areas check procedure
            // (replacement pixel should always touch previous, current and next pixel).
            const Pixel excludedPixel1 = currentPixel;
            const Pixel excludedPixel2 = ChainCodeFunctions::chainCodeMove(noisyChainCode.type, first, excludedPixel1);
            const Pixel excludedPixel3 = ChainCodeFunctions::chainCodeMove(noisyChainCode.type, second, excludedPixel2);
            const std::vector<Pixel> excludedPixels({ excludedPixel1, excludedPixel2, excludedPixel3 });

            if (excludedPixel2 == startPixel) {
                //continue;
            }

            // Checking whether replacement chain code segment would introduce any self-touching areas.
            // If there would be no self-touching areas, we praise the Lord and make some NOISE!
            if (!wouldNoiseCauseSelfTouchingArea(noisyChainCode.type, currentPixel, replacement, borderPixels, excludedPixels,01)) {
            //if (!wouldNoiseCauseSelfTouchingArea(noisyChainCode.type, Pixel(3, 1), replacement, borderPixels, excludedPixels, 1)) {
                // Calculating previous and current pixel sequences from chain code segments.
                const std::vector<Pixel> previousPixels = chainCodeSegmentToPixels(noisyChainCode.type, currentPixel, std::vector<short>({ noisyChainCode.code.begin() + i, noisyChainCode.code.begin() + i + 2 }));
                const std::vector<Pixel> newPixels = chainCodeSegmentToPixels(noisyChainCode.type, currentPixel, replacement);

                // Replacing the original chain code segment with the noisy one.
                noisyChainCode.code.erase(noisyChainCode.code.begin() + i, noisyChainCode.code.begin() + i + 2);
                noisyChainCode.code.insert(noisyChainCode.code.begin() + i, replacement.begin(), replacement.end());

                // Erasing obsolete pixels and introducing new pixels to the set of border pixels.
                for (const Pixel& previousPixel : previousPixels) {
                    borderPixels.erase(previousPixel);
                }
                for (const Pixel& newPixel : newPixels) {
                    borderPixels.insert(newPixel);
                    i++;  // Moving past the introduced noise.
                }

                //for (uint i = 0; i < noisyChainCode.code.size(); i++) {
                //    std::cout << noisyChainCode.code[i];
                //}
                //std::cout << std::endl;

                // Current pixel is set to the last value of the noisy chain code segment.
                currentPixel = newPixels.back();
            }
        }

        // Moving in the right direction.
        previousPixel = currentPixel;
        currentPixel = ChainCodeFunctions::chainCodeMove(noisyChainCode.type, noisyChainCode.code[i], currentPixel);
    }

    return noisyChainCode;
}

std::vector<Pixel> ChainCodeNoise::chainCodeSegmentToPixels(const ChainCodeType& type, const Pixel& startPixel, const std::vector<short>& sequence) {
    std::vector<Pixel> pixels;
    
    // Last order is pruned in order to prevent to obtain a pixel that is already a part of a chain code.
    const std::vector<short> prunedSequence({ sequence.begin(), sequence.end() - 1 });

    // Moving the pixel according to the order sequence.
    Pixel currentPixel = startPixel;
    for (const short direction : prunedSequence) {
        // Movement of a pixel.
        currentPixel = ChainCodeFunctions::chainCodeMove(type, direction, currentPixel);

        // If we stumble upon a negative coordinate, we stumbled upon a shite.
        //if (currentPixel.x < 0 || currentPixel.y < 0) {
        //    throw ("Negative pixel coordinates.");
        //}

        // Adding a new pixel to the vector.
        pixels.push_back(currentPixel);
    }

    return pixels;
}

bool ChainCodeNoise::wouldNoiseCauseSelfTouchingArea(const ChainCodeType& type, const Pixel& startPixel, const std::vector<short>& noiseSequence, const std::unordered_set<Pixel>& borderPixels, const std::vector<Pixel>& excludedPixels, const int vicinity) {
    // Last order is pruned in order to prevent to check a pixel that is already a part of a chain code.
    const std::vector<short> prunedNoiseSequence({ noiseSequence.begin(), noiseSequence.end() - 1 });

    // Moving the pixel according to the order sequence.
    Pixel currentPixel = startPixel;
    for (const short direction : prunedNoiseSequence) {
        // Movement of a pixel.
        currentPixel = ChainCodeFunctions::chainCodeMove(type, direction, currentPixel);

        // Checking the vicinity of the pixel.
        for (int y = -vicinity; y <= vicinity; y++) {
            for (int x = -vicinity; x <= vicinity; x++) {
                if (type == ChainCodeType::F4 && (x + y) % 2 == 0 && std::abs(x) + std::abs(y) != 0) {
                    continue;
                }

                // Obtaining a pixel that is being checked.
                const Pixel checkPixel(currentPixel.x + x, currentPixel.y + y);

                // If the pixel is in the list of excluded pixels, we continue our journey.
                if (checkPixel == excludedPixels[0] || checkPixel == excludedPixels[1] || checkPixel == excludedPixels[2]) {
                    continue;
                }
                // If a pixel in the vicinity is found, our journey is over, as we stumbled upon
                // a self-touching area. According to some sources, self-touching is bad.
                else if (borderPixels.find(checkPixel) != borderPixels.end()) {
                    return true;
                }
            }
        }
    }

    // If none of the vicinity pixels has been found on the object border, there is no self-touching.
    return false;
}

void ChainCodeNoise::saveChainCodeImage(const std::vector<ChainCode>& chainCodes, const uint iteration, const std::string name, const uint probability) {
    // Transforming chain codes into coordinates.
    const auto& [coordinates, maxXCoordinate, maxYCoordinate] = ChainCodeFunctions::calculateCoordinates(chainCodes);

    // Drawing the coordinates on PixMap.
    const uint scale = 2;
    const uint padding = 1;
    QPixmap pix(scale * (maxXCoordinate + 2 * padding), scale * (maxYCoordinate + 2 * padding));
    pix.fill(QColor("white"));
    QPainter paint(&pix);
    paint.setPen(QColor(0, 0, 0, 255));
    for (const std::vector<Pixel>& currentCoordinates : coordinates) {
        for (const Pixel& coordinate : currentCoordinates) {
            paint.drawRect(scale * (coordinate.x + padding), scale * (maxYCoordinate - (coordinate.y) + padding), 1, 1);
        }
    }

    std::stringstream ss;
    ss << "./Test/";
    QDir().mkdir(QString::fromUtf8(ss.str()));
    ss << name << "/";
    QDir().mkdir(QString::fromUtf8(ss.str()));
    ss << probability << "/";
    QDir().mkdir(QString::fromUtf8(ss.str()));
    ss << name << iteration + 1 << ".png";
    pix.toImage().save(QString::fromUtf8(ss.str()));
}

void ChainCodeNoise::analyzeNoise(const std::vector<ChainCode>& noisyChainCodes, const uint iteration, const std::string& name, const uint probability) const {
    // Noise analysis.
    NoiseAnalyzer noiseAnalyzer(m_OriginalChainCodes);
    //const double averageDistance = noiseAnalyzer.analyzeNoise(noisyChainCodes, NoiseAnalysisType::euclidean);
    //std::cout << "Average distance after " << iteration + 1 << " iterations: " << averageDistance << std::endl;
    const double fractalDimension = noiseAnalyzer.fractalDimension(noisyChainCodes);
    std::cout << "Fractal dimension after " << iteration + 1 << " iterations: " << fractalDimension << std::endl;
    
    std::stringstream ss;
    ss << "./Test/";
    QDir().mkdir(QString::fromUtf8(ss.str()));
    ss << name << "/";
    QDir().mkdir(QString::fromUtf8(ss.str()));
    ss << probability << "/" << name << ".csv";

    std::ofstream out(ss.str(), std::ios_base::app);
    //out << averageDistance << ",";
    out << fractalDimension << ",";
}



ChainCodeNoise::ChainCodeNoise(const std::vector<ChainCode>& originalChainCodes) : m_OriginalChainCodes(originalChainCodes) {
    // Initilizing random component.
    m_Generator = std::mt19937(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    m_Random = std::uniform_real_distribution<>(0.0, 1.0);
}

ChainCodeNoise& ChainCodeNoise::operator=(const ChainCodeNoise& chainCodeNoise) {
    this->m_OriginalChainCodes = chainCodeNoise.m_OriginalChainCodes;
    return *this;
}

std::vector<ChainCode> ChainCodeNoise::applyNoise(const std::vector<ChainCode>& chainCodes, const std::vector<Pixel>& startPixels, std::unordered_set<Pixel>& borderPixels, const double noiseProbability, const uint numberOfIterations, const std::string& name) {
    std::vector<ChainCode> noisyChainCodes = chainCodes;

    //{
    //    std::stringstream ss;
    //    ss << "./Test/";
    //    QDir().mkdir(QString::fromUtf8(ss.str()));
    //    ss << name << "/";
    //    QDir().mkdir(QString::fromUtf8(ss.str()));
    //    ss << static_cast<uint>(100 * noiseProbability) << "/";
    //    QDir().mkdir(QString::fromUtf8(ss.str()));
    //    ss << name << ".csv";
    //    std::ofstream out(ss.str(), std::ios_base::trunc);
    //    if (chainCodes[0].type == ChainCodeType::F4) {
    //        out << "F4" << ",";
    //    }
    //    else if (chainCodes[0].type == ChainCodeType::F8) {
    //        out << "F8" << ",";
    //    }
    //}

    //saveChainCodeImage(noisyChainCodes, -1, name, 100 * noiseProbability);


    for (uint iteration = 0; iteration < numberOfIterations; iteration++) {
        auto start = std::chrono::high_resolution_clock::now();
        for (uint i = 0; i < chainCodes.size(); i++) {
            noisyChainCodes[i] = addNoiseToChainCode(noisyChainCodes[i], startPixels[i], borderPixels, noiseProbability, numberOfIterations);
        }

        uint segmentCount = 0;
        for (const ChainCode& chainCode : noisyChainCodes) {
            segmentCount += chainCode.code.size();
        }

        auto midtime = std::chrono::high_resolution_clock::now();
        auto currrentTime = std::chrono::duration_cast<std::chrono::milliseconds>(midtime - start).count();
        std::cout << "Progress: " << iteration + 1 << "/" << numberOfIterations << ", Number of CC: " << segmentCount << " (" << (currrentTime / 1000.0) << " s), " << (segmentCount/(currrentTime/1000.0)) << std::endl;

        //saveChainCodeImage(noisyChainCodes, iteration, name, 100 * noiseProbability);
        //analyzeNoise(noisyChainCodes, iteration, name, 100 * noiseProbability);
    }

    return noisyChainCodes;
}