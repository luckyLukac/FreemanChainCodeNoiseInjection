#pragma once

#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>

#include "Constants.hpp"
#include "Pixel.hpp"


/// <summary>
/// Type of the chain code (F8, F4, VCC...).
/// </summary>
enum class ChainCodeType {
	F8,
	F4
};


/// <summary>
/// Structure for storing a chain code.
/// </summary>
struct ChainCode {
	std::vector<short> code;  // Sequence of chain code commands.
	ChainCodeType type;		  // Type of the chain code (F8, F4, VCC...).
	int startX;				  // X start coordinate.
	int startY;				  // Y start coordinate.

	/// <summary>
	/// Constructor of the structure.
	/// </summary>
	/// <param name="chainCode">: string that forms the F8 chain code</param>
	/// <param name="type">: type of the chain code (F8, F4, VCC...)</param>
	/// <param name="startX">: X start coordinate</param>
	/// <param name="startY">: Y start coordinate</param>
	ChainCode(const std::string& chainCode, const ChainCodeType type, const int startX, const int startY);

	/// <summary>
	/// Transforming the chain code to the vector of coordinates.
	/// </summary>
	/// <returns>Vector of pixels</returns>
	std::vector<Pixel> toCoordinates() const;
};


namespace ChainCodeFunctions {
	/// <summary>
	/// Embedding the chain code into a raster space and calculation of its coordinates.
	/// </summary>
	/// <param name="chainCodes">: vector of chain codes</param>
	/// <returns>(vector of vectors of coordinates (by each chain code), maximal X coordinate, maximal Y coordinate)</returns>
	std::tuple<std::vector<std::vector<Pixel>>, uint, uint> calculateCoordinates(const std::vector<ChainCode>& chainCodes);

	/// <summary>
	/// Transforming a vector of pixels to a hash table.
	/// </summary>
	/// <param name="coordinates">: vector of vectors of coordinates</param>
	/// <param name="maxCoordinate">: maximum coordinate</param>
	/// <returns>Unordered set of pixels</returns>
	std::unordered_set<Pixel> coordinatesToSet(const std::vector<std::vector<Pixel>>& coordinates, const uint maxCoordinate);

	/// <summary>
	/// Generating a pixel field.
	/// </summary>
	/// <param name="coordinates">: vector of border pixels (coordinates)</param>
	/// <param name="maxCoordinate">: maximal coordinate</param>
	/// <returns>Generated pixel field</returns>
	PixelField generatePixelField(const std::vector<Pixel>& coordinates, const uint maxCoordinate);

	/// <summary>
	/// Move along the F8 chain code with the given direction.
	/// </summary>
	/// <param name="type">: chain code type</param>
	/// <param name="direction">: F8 direction [0-7]</param>
	/// <param name="startPixel">: starting pixel</param>
	/// <returns>Next pixel</returns>
	Pixel chainCodeMove(const ChainCodeType& type, const uint direction, const Pixel& startPixel);

	/// <summary>
	/// Transforming pixel coordinates to a unique index.
	/// </summary>
	/// <param name="pixel">: input pixel</param>
	/// <param name="size">: size of the one side of the grid</param>
	/// <returns>Unique index</returns>
	uint pixelToUniqueIndex(const Pixel& pixel, const uint size);

	/// <summary>
	/// Calculating extreme pixels in a coordinate vector.
	/// </summary>
	/// <param name="coordinates">: vector of vectors of pixels (by each chain code)</param>
	/// <returns>Pixel(xMin, yMin), Pixel(xMax, yMax)</returns>
	std::pair<Pixel, Pixel> extremeCoordinates(const std::vector<std::vector<Pixel>>& coordinates);
}