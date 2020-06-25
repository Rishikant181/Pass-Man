// This header files contains all the other headers as well as packages use by the program
#pragma once

// Including packages
#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <filesystem>

// Including headers
#include "Crypto.h"
#include "Helper.h"
#include "Logger.h"
#include "Mailman.h"
#include "MangMethods.h"
#include "PassMan.h"
#include "SecMethods.h"

// Objects of various classes used in the program
passMan* pm;														// To store passMan object
logMan* lm;															// To store logMan object
cryptMan* cm;														// To store cryptMan object
secMan* sm;															// To store secMan object