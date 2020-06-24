#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>
#include <ctime>
#include "PassMan.h"

// Declaring class Logger
class logMan {
private:
	// Private member data
	std::string workDir;											// To store working directory
	std::ifstream inpLogFile;										// To store input file
	std::ofstream outLogFile;										// To store output file
	// Private member method
	// Method to check if logs from current date exist
	bool dateExists(std::string date);

public:
	// Member method
	// Constructor
	logMan(passMan& ob);
	// Method to log data with message
	void logData(std::string msg);
};