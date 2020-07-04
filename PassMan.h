#pragma once

// Declaring class passMan
class passMan {	
private:
	// Member data
	std::string workDir;											// To store working directory
	std::string dataLocation;										// To store location of stored data
	std::string refLocation;										// To store location of reference name data
	std::string logDir;												// To store logs directory name
	// Member method
public:
	// Member method
	passMan(std::string dir);										// Constructor
	// Getter methods
	std::string getDataLoc();										// Method to get dataLoc
	std::string getRefLoc();										// Method to get refLoc
	std::string getLogDir();										// Method to get logDir
};