// This file contains various methods for management of stored passwords
#include <fstream>
#include <string>
#include <iostream>
#include "Crypto.h"
#include "Helper.h"

using namespace std;

// Method to extract encrypted password from line
string getPassFromLine(string inpLine) {
	int comCount = 0;								// To store number of commas passed
	int lineLen = inpLine.length();					// To store length of line
	string encryptedPass = "";						// To store encrypted password
	for (int i = 0; i < lineLen; i++) {
		// Keeping track of comma count
		if (inpLine[i] == ',') {
			comCount += 1;
		}
		// If the comma before encrypted password
		else if (comCount == 2) {
			// Extracting substring of line beginning from current position after comma to end
			encryptedPass = inpLine.substr(i);
			encryptedPass = encryptedPass.substr(0, encryptedPass.length());
			return encryptedPass;
		}
	}
}

// Method to get non-optional input
string getReqInput() {
	string inpVal;									// To store input string
	while (true) {
		// Take input
		getline(cin, inpVal);
		// If valid input
		if (inpVal.compare("") != 0) {
			return inpVal;
		}
		// If invalid input
		else {
			cout << "Please enter a valid input !" << endl;
		}
	}
}

// Method to add a new password to the database
void addPass(ofstream& outData, int enKeyMul, int enKeyAdd) {
	// Declaring vars
	string refName;										// To store reference name of a password
	string passCom;										// To store commments about passwrds
	string actPass;										// To store the actual password
	string conPass;										// To store the confirmation password

	cout << "Enter reference name for password :  ";
	refName = getReqInput();
	cout << "Comments (Optional) : ";
	getline(cin, passCom);
	cout << "Enter password : ";
	actPass = getReqInput();
	cout << "Confirm password : ";
	conPass = getReqInput();

	// Checking if conPass == actPass
	if (conPass.compare(actPass) == 0) {
		// Adding data to database
		outData << refName << "," << passCom << "," << inpEncrypt(actPass, enKeyMul, enKeyAdd) << endl;
		outData.clear();
		cout << "Successfully stored password !" << endl;
	}
	else {
		cout << "Confirmation failed ! Please try again !";
	}
}

// Method to find an existing password from database using reference name
string getPass(ifstream& inData, string searchItem) {
	string line;									// To store each line of inData
	string enPass;									// To store the encrypted password found from database
	bool isFound = false;							// To store whether refName found in database or not
	searchItem = toLower(searchItem);
	// Opening database file
	do {
		// Getting each line from inData
		getline(inData, line);
		line = toLower(line);

		// Checking for match case
		if (searchItem.compare(line.substr(0, searchItem.length())) == 0) {
			// Flagging as found
			isFound = true;
			// Getting password for associated reference name
			enPass = getPassFromLine(line);
			return enPass;
		}
	} while (line.compare("") != 0);
	// If not found
	if (isFound == false) {
		return "Not Found";
	}
}