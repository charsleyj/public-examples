// CSC8501-Coursework.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Author:	James Charsley
// Date:	4/10/2019

#include <iostream>
#include <sstream>
#include <algorithm>
#include <regex>
#include "UserInputHandler.h"
#include "Configuration.h"
#include "FileIO.h"
#include "Solution.h"
#include "Factory.h"
#include "Solver.h"

using namespace std;

void write15File(int num, vector<Configuration*> configs) {
	
	FileIO file;
	inputFilename("Filename to write: ", file, WRITE);
	file << num << "\n";
	for (int i = 0; i < num; ++i) {
		file << *configs[i] << "\n";
	}
	file.close();
}

vector<Configuration> read15File() {
	FileIO file;
	inputFilename("Filename to read: ", file, READ);
	int numConfigs;
	vector<Configuration> configs;

	numConfigs = file.readInt();
	file.skipLine();
	for (int i = 0; i < numConfigs; ++i) {
		configs.emplace_back(Configuration(file.readConfig()));
	}
	file.close();
	return configs;
}

void solve15File() {
	Solver solver;
	try {
		solver.addConfigurations(read15File());
	} catch (runtime_error e) {
		cout << e.what() << "\n";
		return;
	}

	FileIO file;
	inputFilename("Filename to write: ", file, WRITE);

	bool incEmpty = inputBool("Include empty square? (y/n): ");
	bool incSinglePartials = inputBool("Include initial partial continuations? (y/n): ");
	bool incAllPartials = inputBool("Include all partial continuations? (y/n):");

	file << solver.getConfigs().size() << "\n";
	solver.solveConfigs(incEmpty, incSinglePartials, incAllPartials);

	bool doPrint = inputBool("Do you want to print the solutions? (y/n): ");
	for (auto sol : solver.getSolutions()) {
		if(doPrint) cout << sol << "\n";
		file << sol << "\n";
	}
	file.close();
}

void inputConfiguration(Factory& factory) {

	int sizeX = inputInt("Horizontal size (3-1000): ", 3, 1000);
	int sizeY = inputInt("Vertical size (3-1000): ", 3, 1000);

	Configuration config;
	int* input = new int[sizeX*sizeY-1];
	bool isInputting = true;
	while (isInputting) {

		cout << "Please Enter " << sizeX * sizeY - 1 << " tile numbers (1-1000000).\n";
		cout << "blocks will appear left to right, up to down.\n\n";

		inputUniqueIntArray(1, 1000000, input, sizeX * sizeY - 1);
		config = Configuration(sizeX, sizeY, input);
		cout << "\n" << config << '\n';

		isInputting = !inputBool("Is this ok? (y/n): ");
	}
	delete[] input;
	factory.inputConfig(config);
}

void generateConfigurations(Factory& factory) {

	int sizeX = inputInt("Horizontal size (3-1000): ", 3, 1000);
	int sizeY = inputInt("Vertical size (3-1000): ", 3, 1000);
	int numBlocks = sizeX * sizeY - 1;
	int range = inputInt("Number range (" + to_string(numBlocks) + "-1000000): ", numBlocks, 1000000);
	int numConfigs = inputInt("Number of configurations (1-1000000): ", 1, 1000000);

	cout << "Generating";
	factory.setRange(range);
	for (int i = 0; i < numConfigs; ++i) {
		factory.generateConfig(sizeX, sizeY);
		if(i % 100 == 0) cout << ".";
	}
	cout << "\n";

}

void outputCreateNewConfigs(int numConfigs) {
	cout << "\n-------- Create New Configurations --------\n\n";
	cout << "Configurations: " << numConfigs << "\n\n";
	cout << "1.\tManually input configuration\n";
	cout << "2.\tGenerate random Configurations\n";
	cout << "3.\tPrint configurations\n";
	cout << "4.\tClear configurations\n";
	cout << "5.\tSave and return\n\n";
}

void createNewConfigurations() {
	Factory factory;
	bool isRunning = true;
	while (isRunning) {
		outputCreateNewConfigs(factory.getNumConfigs());
		int input = inputInt("Please enter menu option (1-4): ", 1, 5);
			switch (input) {
			case 1: inputConfiguration(factory); break;
			case 2: generateConfigurations(factory); break;
			case 3: factory.printConfigs(); break;
			case 4: factory.clearConfigs(); break;
			case 5: isRunning = false; break;
			}
	}
	if(factory.getNumConfigs() != 0)
		write15File(factory.getNumConfigs(), factory.getConfigs());
}

void outputMainMenu() {
	cout << "\n-------- 15-Tile Puzzle Coursework --------\n";
	cout << "1.\tCreate new configurations\n";
	cout << "2.\tSolve configurations in file\n";
	cout << "3.\tExit\n\n";
}

int main() {
	bool isRunning = true;
	while (isRunning) {
		outputMainMenu();
		int input = inputInt("Please enter menu option (1-3): ", 1, 3);
		switch (input) {
		case 1: createNewConfigurations(); break;
		case 2: solve15File(); break;
		case 3: isRunning = false; break;
		}

		cout << '\n';
	}
	return 0;
}