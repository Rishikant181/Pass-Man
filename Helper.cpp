// This file contains various helper methods

#include <string>

using namespace std;

// Method to convert a string to lowercase
string toLower(string inpStr) {
	int strLen = inpStr.length();				// To store length of string
	for (int i = 0; i < strLen; i++) {
		inpStr[i] = tolower(inpStr[i]);
	}
	return inpStr;
}