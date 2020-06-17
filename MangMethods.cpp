// This file contains various methods for management of stored passwords
#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
#include "Crypto.h"
#include "Helper.h"
#include "PassMan.h"

using namespace std;

// Method to check for duplicate refName, return false if not present and true otherwise
bool checkRefName(ifstream& inpFile, string inpRefName) {
	string line;									// To store each line of inpFile
	string refName;									// To store extracted refName from file
	inpRefName = toLower(inpRefName);

	// Getting each line from file
	while (true) {
		getline(inpFile, line);
		// Checking for blank line, i.e, end of file
		if (line.compare("") == 0) {
			return false;
		}
		// For non-blank line
		refName = toLower(line.substr(0, line.find(',')));
		
		// Checking for match
		if (refName.compare(inpRefName) == 0) {
			return true;
		}
	}
	// For not present
	return false;
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
void addPass() {
	string passCom;										// To store commments about passwrds
	string actPass;										// To store the actual password
	string conPass;										// To store the confirmation password

	cout << "Comments (Optional) : ";
	getline(cin, passCom);
	cout << "Enter password : ";
	actPass = getReqInput();
	cout << "Confirm password : ";
	conPass = getReqInput();

	// Checking if conPass == actPass
	if (conPass.compare(actPass) == 0) {
		// Adding data to database
		outData << inpEncrypt(actPass) << " " << passCom << "\n";
		cout << "Successfully stored password !" << endl;
	}
	else {
		cout << "Confirmation failed ! Please try again !";
	}
}

// Method to get encrypted pass from database
string getPass(string refName) {
	string enPass;									// To store encrypted password
	// If reference name not found
	if (filesystem::exists(dataLocation + refName + ".pass") == false) {
		// Exitting from program
		return "NA";
	}
	inpData.open(dataLocation + refName + ".pass");
	getline(inpData, enPass);
	inpData.clear();
	inpData.close();
	return enPass;
}

// Method to get the list of all passwords stored by pass-man and displaying only refName and comments
void getList() {
	string refName;									// To store file name
	string passCom;									// To store comment
	// Iterating through directory
	for (auto& file : filesystem::directory_iterator(dataLocation)) {
		// Getting refName
		refName = file.path().string().substr(dataLocation.length());
		refName = refName.substr(0, refName.length() - 5);
		
		// Getting comment from file
		inpData.open(dataLocation + refName + ".pass");
		getline(inpData, passCom);
		inpData.clear();
		inpData.close();
		passCom = passCom.substr(passCom.find(' ') + 1);

		// Displaying
		cout << "Reference Name : " << refName << endl;
		cout << "Comments       : " << passCom << "\n" << endl;
	}
}