#pragma once

// Declaring class Logger
class logMan {
private:
	// Private member data
	std::string logDir;												// To store log directory name
	std::ifstream inpLogFile;										// To store input file
	std::ofstream outLogFile;										// To store output file

public:
	// Member method
	// Constructor
	logMan(std::string dir);
	// Method to log data with message
	void logData(std::string msg);
};