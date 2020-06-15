// This file contains various methods for management of stored passwords
#include <fstream>
#include <string>
#include <iostream>
#include "Crypto.h"

using namespace std;

// Declaring global vars
string refName;										// To store reference name of a password
string passCom;										// To store commments about passwrds
string actPass;										// To store the actual password
string conPass;										// To store the confirmation password

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