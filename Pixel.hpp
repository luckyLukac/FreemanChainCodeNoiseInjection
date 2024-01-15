#pragma once

#include "Constants.hpp"


/// <summary>
/// Pixel structure.
/// </summary>
struct Pixel {
	int x;
	int y;

	Pixel();
	Pixel(const int x, const int y);

	bool operator == (const Pixel& pixel) const;
};


/// <summary>
/// Pixel hash function (to be used for unordered set).
/// </summary>
template<>
struct std::hash<Pixel> {
	int operator () (const Pixel& pixel) const {
		return std::hash<int>()(10000 * pixel.y + pixel.x);
	}
};