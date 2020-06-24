// This method contains the logger class to log data

#include <iostream>
#include <filesystem>
#include <fstream>
#include <ctime>
#include "Logger.h"
#include "PassMan.h"

// Defining Logger class

// Defining dateExists method
bool logMan::dateExists(std::string date) {
	// Checking if logs from given date exist and returning val
	return std::filesystem::exists(date + ".log");
}

// Defining constructor
logMan::logMan(passMan &ob) {
	// Initialising vars
	// Private member data
	logDir = "Logs\\";
	workDir = ob.getStringMemberData("workdir");
}

// Defining logData method
void logMan::logData(std::string msg) {
	// Declaring vars
	char nowDateTime[26];											// To store date-time as char array
	bool isDateLogged;												// To store if date already logged
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

	// Checking if log of given date exists
	isDateLogged = dateExists(nowDate);

	outLogFile.open(workDir + logDir + nowDate + ".log", std::ios_base::app);
	outLogFile << nowTime + "	->	" + msg << std::endl;
	outLogFile.clear();
	outLogFile.close();
}