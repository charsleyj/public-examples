// File:	Configuration.cpp
// Author:	James Charsley (150338217)
// Date:	14/10/2019
// Desc:	Contains the class definition which holds the information for a configuration

#include "Configuration.h"
#include <algorithm>
#include <numeric>
#include <exception>
#include <random>

using namespace std;

Configuration::Configuration() :
	xLength(4),
	yLength(4) {
	blocks = new int[xLength * xLength - 1];
	iota(blocks, blocks + xLength * xLength - 1, 1);
}

Configuration::Configuration(int sizeX, int sizeY, int* elements) :
	xLength(sizeX),
	yLength(sizeY) {
	blocks = new int[xLength * yLength - 1];
	copy(elements, elements + (xLength * yLength - 1), blocks);
}

Configuration::Configuration(const Configuration& src) :
	xLength(src.xLength),
	yLength(src.yLength) {
	blocks = new int[xLength * yLength - 1];
	copy(src.blocks, src.blocks + xLength * yLength - 1, blocks);
}

Configuration::~Configuration() {
	delete[] blocks;
}

Configuration* Configuration::operator=(const Configuration& rhs) {
	delete[] blocks;
	xLength = rhs.xLength;
	yLength = rhs.yLength;
	blocks = new int[xLength * yLength - 1];
	copy(rhs.blocks, rhs.blocks + xLength * yLength - 1, blocks);
	return this;
}

int Configuration::sizeX() const { 
	return xLength;
}

int Configuration::sizeY() const {
	return yLength;
}

int Configuration::numSpaces() const {
	return sizeX() * sizeY();
}

int Configuration::numBlocks() const {
	return numSpaces() - 1;
}

int Configuration::getBlock(int x, int y) const {
	if (x >= sizeX() || x < 0 || y >= sizeY() || y < 0 || (x == sizeX()-1 && y == sizeY()-1))
		throw invalid_argument("Arguments are outside the bounds of the configuration.");
	return blocks[y * sizeX() + x];
}

int Configuration::getBlock(int i) const {
	if (i < 0 || i>numBlocks() - 1)
		throw invalid_argument("Argument is outside the bounds of the configuration.");
	return blocks[i];
}

int* Configuration::getBlocks() const {
	int* result = new int[numBlocks()];
	copy(blocks, blocks + numBlocks(), result);
	return result;
}

bool Configuration::isReachable(Configuration other) {
	int swaps = 0;
	for (int i = 0; i < numBlocks()-1; ++i) {
		for (int j = i + 1; j < numBlocks(); ++j) {
			if (other.blocks[j] == blocks[i]) {
				other.swap(i, j);
				swaps += 1;
			}
		}
	}
	return (swaps % 2) == 0;
}

ostream& operator<<(std::ostream& out, const Configuration& config) {
	for (int y = 0; y < config.sizeY(); ++y) {
		for (int x = 0; x < config.sizeX(); ++x) {
			if (x != config.sizeX()-1 || y != config.sizeY()-1)
				out << config.blocks[config.sizeX() * y + x] << '\t';
		}
		out << '\n';
	}
	return out;
}

bool Configuration::swap(int elem1, int elem2) {
	int temp = blocks[elem1];
	blocks[elem1] = blocks[elem2];
	blocks[elem2] = temp;
	return true;
}

bool Configuration::outOfBounds(int x, int y) const {
	return	x	<	0		||
			x	>=	xLength	||
			y	<	0		||
			y	>=	yLength	||
			(x == xLength - 1 && y == yLength - 1);
}