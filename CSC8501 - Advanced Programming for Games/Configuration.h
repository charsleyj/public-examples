// File:	Configuration.h
// Author:	James Charsley (150338217)
// Date:	14/10/2019
// Desc:	Contains the class declaration which holds the information for a configuration

#pragma once
#include <iostream>

class Configuration {

public:
	Configuration();
	Configuration(int sizeX, int sizeY, int* elements);
	Configuration(const Configuration& src);
	~Configuration();

	Configuration* operator=(const Configuration& rhs);
	int getBlock(int x, int y) const;
	int getBlock(int i) const;
	int* getBlocks() const;

	inline int sizeX() const;
	inline int sizeY() const;
	inline int numSpaces() const;
	inline int numBlocks() const;

	bool isReachable(Configuration other);

	friend std::ostream& operator<<(std::ostream& out, const Configuration& config);

private:
	int* blocks;
	int xLength;
	int yLength;

	bool swap(int elem1, int elem2);
	bool outOfBounds(int x, int y) const;
};