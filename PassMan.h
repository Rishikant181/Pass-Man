#pragma once

// Declaring class passMan
class passMan {	
private:
	// Member data
	std::string workDir;											// To store working directory
	std::string dataLocation;										// To store location of stored data
	std::string refLocation;										// To store location of reference name data
	std::string logDir;												// To store logs directory name
	std::string auFileName;											// To store auth key file name
	std::string mailIdLoc;											// To store mail-id file location
	// Member method
	int selfDestruct();												// Method to free memory and exit
public:
	// Member method
	passMan(std::string dir);										// Constructor
	std::string getStringData(std::string dName);					// Method to get private string data
};