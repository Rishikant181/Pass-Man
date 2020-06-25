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
extern passMan* pm;														// To store passMan object
extern logMan* lm;															// To store logMan object
extern cryptMan* cm;														// To store cryptMan object
extern secMan* sm;															// To store secMan object