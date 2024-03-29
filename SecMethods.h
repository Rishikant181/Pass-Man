#pragma once

// Declaring class secMan
class secMan {
private:
	// Member data
	std::string authPass;												// To store authorization password

public:
	// Member methods
	secMan();															// Constructor
	bool checkAuth();													// Method to check authentication
	bool changeAuthKey();												// Method to change authentication key
	bool failAuthMail();												// Method to send mail incase of failed authentication
	std::string getStringData(std::string data);						// Method to retrieve private data
};