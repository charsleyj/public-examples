// File:	ConfigSolution.cpp
// Author:	James Charsley (150338217)
// Date:	21/10/2019
// Desc:	Definition of class to store configurations and solve them

#include "Solver.h"
#include <algorithm>

using namespace std;

Solver::Solver() :
	currentFactorialSize(1),
	currentFactorialTotal(1) {}

Solver::Solver(const Solver& src) :
	currentFactorialSize(1),
	currentFactorialTotal(1) {
	for (auto config : src.configs) {
		configs.emplace_back((config));
	}
	for (auto solution : src.solutions) {
		solutions.emplace_back((solution));
	}
}

Solver::~Solver() {}

Solver* Solver::operator=(const Solver& rhs) {
	configs.clear();
	solutions.clear();
	for (auto config : rhs.configs) {
		configs.emplace_back((config));
	}
	for (auto solution : rhs.solutions) {
		solutions.emplace_back((solution));
	}
	return this;
}

vector<Configuration> Solver::getConfigs() {
	return configs;
}

vector<Solution> Solver::getSolutions() {
	return solutions;
}

void Solver::addConfiguration(const Configuration& config) {
	configs.emplace_back((config));
}

void Solver::addConfigurations(vector<Configuration> configs) {
	this->configs.insert(this->configs.end(), configs.begin(), configs.end());
}


void Solver::solveConfigs(bool incEmpty, bool incSinglePartials, bool incAllPartials) {
	solutions.clear();
	#pragma loop(hint_parallel(6))
	for (int i = 0; i < configs.size(); ++i) {
		solutions.emplace_back(Solution());
		solutions.back().config = configs[i];
		solutions.back().incSinglePartials = incSinglePartials;
		solutions.back().incAllPartials = incAllPartials;
		solveConfig(solutions.back(), incEmpty);
	}
}



void Solver::solveConfig(Solution& sol, bool incEmpty) {
	int* blocks = sol.config.getBlocks();
	int numSpaces = sol.config.numSpaces();
	sort(blocks, blocks + sol.config.numBlocks());

	sol.rows = solveRows(blocks, sol.config.sizeX(), sol.config.sizeY(), numSpaces, incEmpty);
	sol.columns = solveRows(blocks, sol.config.sizeY(), sol.config.sizeX(), numSpaces, incEmpty);
	sol.reverseRows = sol.rows;
	sol.reverseColumns = sol.columns;

	if (sol.incSinglePartials) {
		sol.singlePartial2 = solveSinglePartial(sol.config, 2);
		sol.singlePartial3 = solveSinglePartial(sol.config, 3);
		sol.singlePartial4 = solveSinglePartial(sol.config, 4);
	}

	if (sol.incAllPartials) {
		sol.allPartial2 = solvePartial(blocks, sol.config.sizeX(), sol.config.sizeY(), numSpaces, 2);
		sol.allPartial3 = solvePartial(blocks, sol.config.sizeX(), sol.config.sizeY(), numSpaces, 3);
		sol.allPartial4 = solvePartial(blocks, sol.config.sizeX(), sol.config.sizeY(), numSpaces, 4);
	}
}

bigint Solver::solveRows(int* blocks, int rowSize, int numRows, int numSpaces, bool incEmpty) {
	int numRuns = calcNumRuns(blocks, blocks + numSpaces, rowSize);
	int remainingPieces = numSpaces - 1 - rowSize;
	bigint result =	factorial(remainingPieces) * numRuns * (numRows - 1) / 2;

	if (incEmpty) {
		numRuns = calcNumRuns(blocks, blocks + numSpaces, rowSize - 1);
		result += factorial(remainingPieces) * (numSpaces - rowSize) * numRuns / 2;
	}

	return result;
}



bigint Solver::solvePartial(int* blocks, int sizeX, int sizeY, int numSpaces, int partialSize) {
	int numRuns = calcNumRuns(blocks, blocks + numSpaces, partialSize);
	int remainingPieces = numSpaces - 1 - partialSize;
	int partialPositionsX = sizeX >= partialSize ? (sizeY * (sizeX - partialSize + 1) - 1) : 0;
	int partialPositionsY = sizeY >= partialSize ? (sizeX * (sizeY - partialSize + 1) - 1) : 0;
	return factorial(remainingPieces) * 2 * numRuns * (partialPositionsX + partialPositionsY) / 2;
}



bigint Solver::solveSinglePartial(const Configuration& config, int partialSize) {
	bigint total(0);
	total += solveSinglePartialRow(config, config.sizeX(), partialSize, [](const Configuration& con, int i) {
		return con.getBlock(i);
		});
	total += solveSinglePartialRow(config, config.sizeY(), partialSize, [](const Configuration& con, int i) {
		return con.getBlock((con.sizeX() * (i % con.sizeY())) + (i / con.sizeY()));
		});
	return total;
}

bigint Solver::solveSinglePartialRow(const Configuration& config, int size, int partialSize, function<int(const Configuration&,int)> getBlock) {

	bigint total = 0;
	int dist = 0;
	int inc = 1;
	for (int i = 1; i < config.numBlocks(); ++i) {

		if (i % size == 0) {
			dist = 0;
			continue;
		}
		if (getBlock(config, i) == getBlock(config, i - 1) + inc)
			dist++;
		else
			dist = (getBlock(config, i) == getBlock(config, i - 1) + (inc *= -1)) ? 1 : 0;

		if (dist == partialSize - 1) {
			total += 1;
			dist -= 1;
		}

	}
	return total;
}

int Solver::calcNumRuns(int* blocksBegin, int* blocksEnd, int size) {
	int result = 0;
	while (blocksBegin != blocksEnd - size) {
		if (*blocksBegin + size - 1 == *(blocksBegin + size - 1))
			++result;
		blocksBegin += 1;
	}
	return result;
}

bigint Solver::factorial(int n) {
	if (currentFactorialSize > n) {
		currentFactorialTotal = bigint(1);
		currentFactorialSize = 1;
	}
	while (currentFactorialSize < n) {
		currentFactorialSize += 1;
		currentFactorialTotal *= currentFactorialSize;
	}
	return currentFactorialTotal;
}