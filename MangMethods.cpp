// This file contains various methods for management of stored passwords
#include <fstream>
#include <string>
#include <iostream>
#include "Crypto.h"
#include "Helper.h"
#include "PassMan.h"

using namespace std;

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
void addPass(ofstream& outData) {
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
		outData << refName << "," << passCom << "," << inpEncrypt(actPass) << endl;
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
	while (true) {
		// Getting each line from inData
		getline(inData, line);
		line = toLower(line);

		// Checking if blank line, i.e, end of file
		if (line.compare("") == 0) {
			break;
		}

		// Checking for match case
		if (searchItem.compare(line.substr(0, searchItem.length())) == 0) {
			// Flagging as found
			isFound = true;
			// Getting password for associated reference name
			enPass = subStr(line, ',', 2, 3);
			return enPass;
		}
	}
	// If not found
	if (isFound == false) {
		return "Not Found";
	}
}

// Method to get the list of all passwords stored by pass-man and displaying only refName and comments
void getList(ifstream& inpFile) {
	string line;									// To store each line of inpFile
	string refName;									// To store the reference name
	string passCom;									// To store the comment
	while (true) {
		getline(inpFile, line);
		// If end of file, i.e, blank file
		if (line.compare("") == 0) {
			break;
		}
		// Getting refName
		refName = line.substr(0, line.find(','));
		// Getting comment
		passCom = subStr(line, ',', 1, 2);
		// Displaying
		cout << "Reference Name : " << refName << endl;
		cout << "Comments       : " << passCom << + "\n" << endl;
	}
}