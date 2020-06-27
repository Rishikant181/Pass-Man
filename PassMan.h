#pragma once

// Declaring class passMan
class passMan {	
private:
	// Member data declaration
	std::string workDir;											// To store working directory
	std::string dataLocation;										// To store location of stored data
	std::string refLocation;										// To store location of reference name data
	std::string logDir;												// To store logs directory name
	std::string auFileName;											// To store auth key file name
	std::string mailIdLoc;											// To store mail-id file location
public:
	// Member method declaration
	passMan(std::string dir);										// Constructor
	std::string getStringData(std::string dName);					// Method to get private string data
};