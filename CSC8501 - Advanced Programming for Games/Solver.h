// File:	ConfigSolution.h
// Author:	James Charsley (150338217)
// Date:	21/10/2019
// Desc:	Declaration of class to store configurations and solve them

#pragma once
#include <vector>
#include <functional>
#include "Configuration.h"
#include "Solution.h"
#include "bigint.h"

class Solver {

public:
	Solver();
	Solver(const Solver& src);
	~Solver();

	Solver* operator=(const Solver& rhs);
	
	void addConfiguration(const Configuration& config);
	void addConfigurations(std::vector<Configuration> configs);

	std::vector<Configuration> getConfigs();
	std::vector<Solution> getSolutions();

	void solveConfigs(bool includeEmpty, bool incSinglePartials, bool incAllPartials);
	

private:
	std::vector<Configuration> configs;
	std::vector<Solution> solutions;
	int currentFactorialSize;
	bigint currentFactorialTotal;

	int calcNumRuns(int* blocksBegin, int* blocksEnd, int size);

	void solveConfig(Solution& sol, bool incEmpty);
	bigint solveRows(int* blocks, int rowSize, int numRows, int numSpaces, bool incEmpty);

	bigint solveSinglePartial(const Configuration& config, int partialSize);
	bigint solveSinglePartialRow(const Configuration& config, int size, int partialSize, std::function<int(const Configuration&,int)> getBlock);

	bigint solvePartial(int* blocks, int sizeX, int sizeY, int numSpaces, int partialSize);

	bigint factorial(int n);

};