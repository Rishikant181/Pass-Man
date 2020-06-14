// This file contains various methods for management of stored passwords
#include <fstream>
#include <string>
#include <iostream>
#include "Crypto.h"

using namespace std;

// Declaring global vars
string refName;										// To store reference name of a password
string actPass;										// To store the actual password
string conPass;										// To store the confirmation password

// Method to add a new password to the database
void addPass() {
	cout << "Enter reference name for password :  ";
	getline(cin, refName);
	cout << "Enter password : ";
	getline(cin, actPass);
	cout << "Confirm password : ";
	getline(cin, conPass);
}