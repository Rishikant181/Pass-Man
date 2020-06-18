// This file contains various helper methods
#include <iostream>
#include <string>

// Method to convert a string to lowercase
std::string toLower(std::string inpStr) {
	int strLen = inpStr.length();				// To store length of string
	for (int i = 0; i < strLen; i++) {
		inpStr[i] = tolower(inpStr[i]);
	}
	return inpStr;
}

// Method to get a substring from a string between two occurences of a giver char
std::string subStr(std::string line, char reqChar, int firstOccur, int secondOccur) {
	int numOccur = 0;								// To store occurence number of reqChar
	int lineLen = line.length();					// To store length of line
	std::string outStr = "";						// To store output substring
	for (int i = 0; i < lineLen; i++) {
		// Checking for the char
		if (line[i] == reqChar) {
			numOccur++;
		}
		// Getting required substring
		else if (numOccur >= firstOccur && numOccur < secondOccur) {
			outStr += line[i];
		}
		// Returning value
		else if (numOccur >= secondOccur) {
			return outStr;
		}
	}
	return outStr;
}