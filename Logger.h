#pragma once

// Declaring class Logger
class logMan {
private:
	// Private member data
	std::string logDir;												// To store log directory name

public:
	// Member method
	// Constructor
	logMan(std::string dir);
	// Method to log data with message
	void logData(std::string msg);
};