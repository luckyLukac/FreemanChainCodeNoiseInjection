#include "Pixel.hpp"

Pixel::Pixel() :
	x(0),
	y(0)
{}

Pixel::Pixel(const int x, const int y) :
	x(x),
	y(y)
{}

bool Pixel::operator == (const Pixel& pixel) const {
	return x == pixel.x && y == pixel.y;
}