#include <algorithm>
#include <iostream>

#include "ChainCode.hpp"
#include "ChainCodeNoise.hpp"
#include "Constants.hpp"


ChainCode::ChainCode(const std::string& chainCode, const ChainCodeType type, const int startX, const int startY) :
	type(type),
	startX(startX),
	startY(startY)
{
	// Reading the chain code character by character.
	for (const char& ch : chainCode) {
		uint order = ch - '0';
		code.push_back(order);
	}
}

std::vector<Pixel> ChainCode::toCoordinates() const {
	std::vector<Pixel> coordinates;
	
	// Setting the start point.
	int currentX = startX;
	int currentY = startY;
	coordinates.push_back(Pixel(currentX, currentY));

	// Iterating through the chain code.
	for (const short& chainElement : code) {
		const short direction = chainElement;  // Casting the byte to char.

		// F8.
		if (type == ChainCodeType::F8) {
			// 0 means right.
			if (direction == 0) {
				currentX++;
			}
			// 1 means up and right.
			else if (direction == 1) {
				currentY++;
				currentX++;
			}
			// 2 means up.
			else if (direction == 2) {
				currentY++;
			}
			// 3 means up and left.
			else if (direction == 3) {
				currentY++;
				currentX--;
			}
			// 4 means left.
			else if (direction == 4) {
				currentX--;
			}
			// 5 means down and left.
			else if (direction == 5) {
				currentY--;
				currentX--;
			}
			// 6 means down.
			else if (direction == 6) {
				currentY--;
			}
			// 7 means down and right.
			else if (direction == 7) {
				currentY--;
				currentX++;
			}
		}
		// F4.
		else if (type == ChainCodeType::F4) {
			// 0 means right.
			if (direction == 0) {
				currentX++;
			}
			// 1 means up.
			else if (direction == 1) {
				currentY++;
			}
			// 2 means left.
			else if (direction == 2) {
				currentX--;
			}
			// 3 means down.
			else if (direction == 3) {
				currentY--;
			}
		}
		else {
			throw std::logic_error("Invalid chain code type.");
		}

		// Adding a new coordinate to the vector.
		coordinates.push_back(Pixel(currentX, currentY));
	}

	return coordinates;
}


std::tuple<std::vector<std::vector<Pixel>>, uint, uint> ChainCodeFunctions::calculateCoordinates(const std::vector<ChainCode>& chainCodes) {
	// Transforming each chain code to coordinates.
	std::vector<std::vector<Pixel>> coordinates;
	for (const ChainCode& chainCode : chainCodes) {
		const std::vector<Pixel>& currentCoordinates = chainCode.toCoordinates();
		coordinates.push_back(currentCoordinates);
	}

	// Calculation of extreme coordinates.
	const auto [minPixel, maxPixel] = ChainCodeFunctions::extremeCoordinates(coordinates);
	const uint deltaX = maxPixel.x - minPixel.x;
	const uint deltaY = maxPixel.y - minPixel.y;

	// Moving the bounding box to the left upper corner.
	for (int i = 0; i < coordinates.size(); i++) {
		std::transform(
			coordinates[i].begin(),
			coordinates[i].end(),
			coordinates[i].begin(),
			[&minPixel = minPixel, &maxPixel = maxPixel](const Pixel& pixel) {
				return Pixel(pixel.x - minPixel.x, pixel.y - minPixel.y);
			}
		);
	}

	// Setting the max coordinates.
	uint maxXCoordinate = deltaX + 1;
	if (maxXCoordinate % 2 == 1) {
		maxXCoordinate++;
	}
	uint maxYCoordinate = deltaY + 1;
	if (maxYCoordinate % 2 == 1) {
		maxYCoordinate++;
	}

	return { coordinates, maxXCoordinate, maxYCoordinate };
}

std::unordered_set<Pixel> ChainCodeFunctions::coordinatesToSet(const std::vector<std::vector<Pixel>>& coordinates, const uint maxCoordinate) {
	std::unordered_set<Pixel> hashTable;

	// Transformation of each border pixel.
	for (const std::vector<Pixel>& coordinateVector : coordinates) {
		for (const Pixel& borderPixel : coordinateVector) {
			hashTable.insert(borderPixel);
		}
	}

	return hashTable;
}

PixelField ChainCodeFunctions::generatePixelField(const std::vector<Pixel>& coordinates, const uint maxCoordinate) {
	PixelField pixelField(maxCoordinate, std::vector<bool>(maxCoordinate, false));

	// Setting border pixels to true.
	for (const Pixel& coordinate : coordinates) {
		pixelField[coordinate.y][coordinate.x] = true;
	}

	return pixelField;
}

Pixel ChainCodeFunctions::chainCodeMove(const ChainCodeType& type, const uint direction, const Pixel& startPixel) {
	Pixel pixel(startPixel);

	// F8 chain code.
	if (type == ChainCodeType::F8) {
		if (direction == 0) {
			pixel.x++;
		}
		else if (direction == 1) {
			pixel.x++;
			pixel.y++;
		}
		else if (direction == 2) {
			pixel.y++;
		}
		else if (direction == 3) {
			pixel.x--;
			pixel.y++;
		}
		else if (direction == 4) {
			pixel.x--;
		}
		else if (direction == 5) {
			pixel.x--;
			pixel.y--;
		}
		else if (direction == 6) {
			pixel.y--;
		}
		else if (direction == 7) {
			pixel.x++;
			pixel.y--;
		}
	}
	// F4 chain code.
	else if (type == ChainCodeType::F4) {
		if (direction == 0) {
			pixel.x++;
		}
		else if (direction == 1) {
			pixel.y++;
		}
		else if (direction == 2) {
			pixel.x--;
		}
		else if (direction == 3) {
			pixel.y--;
		}
	}

	return pixel;
}

uint ChainCodeFunctions::pixelToUniqueIndex(const Pixel& pixel, const uint size) {
	return size * pixel.y + pixel.x;
}

std::pair<Pixel, Pixel> ChainCodeFunctions::extremeCoordinates(const std::vector<std::vector<Pixel>>& coordinates) {
	// Initializing coordinates to max and min.
	int xMin = std::numeric_limits<int>::max();
	int yMin = std::numeric_limits<int>::max();
	int xMax = std::numeric_limits<int>::min();
	int yMax = std::numeric_limits<int>::min();

	// Iterating through each coordinate.
	for (const std::vector<Pixel>& currentCoordinates : coordinates) {
		for (uint i = 0; i < currentCoordinates.size(); i++) {
			// If a point X coordinate is smaller than the global
			// minimum, a new global minimum is set.
			if (currentCoordinates[i].x < xMin) {
				xMin = currentCoordinates[i].x;
			}

			// If a point X coordinate is larger than the global
			// maximum, a new global maximum is set.
			if (currentCoordinates[i].x > xMax) {
				xMax = currentCoordinates[i].x;
			}

			// If a point Y coordinate is smaller than the global
			// minimum, a new global minimum is set.
			if (currentCoordinates[i].y < yMin) {
				yMin = currentCoordinates[i].y;
			}

			// If a point Y coordinate is larger than the global
			// maximum, a new global maximum is set.
			if (currentCoordinates[i].y > yMax) {
				yMax = currentCoordinates[i].y;
			}
		}
	}

	return { Pixel(xMin, yMin), Pixel(xMax, yMax) };
}