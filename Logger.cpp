// This method contains the logger class to log data

#include "MasterObjects.h"

// Defining Logger class

// Defining constructor
logMan::logMan(std::string dir) {
	// Initialising vars
	// Private member data
	logDir = dir + "Logs\\";
}

// Defining logData method
void logMan::logData(std::string msg) {
	// Declaring vars
	char nowDateTime[26];											// To store date-time as char array
	std::string nowDateTimeString;									// To store date-time as string
	std::string nowDate;											// To store date as string
	std::string nowTime;											// To store time as string

	// Getting today's date and time
	time_t now = time(NULL);
	
	// Converting time_t to string
	ctime_s(nowDateTime, 26, &now);
	nowDateTimeString = nowDateTime;

	// Getting date and time
	nowDate = nowDateTimeString.substr(4, 7) + nowDateTimeString.substr(20, 4);
	nowTime = nowDateTimeString.substr(11, 8);

	outLogFile.open(logDir + nowDate + ".log", std::ios_base::app);
	outLogFile << nowTime + "	->	" + msg << std::endl;
	outLogFile.clear();
	outLogFile.close();
}