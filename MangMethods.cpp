// This file contains various methods for management of stored passwords

#include "GlobalMembers.h"

// Method to convert store data from one encrytion key type to another
void convertData(cryptMan* oldcm, cryptMan* newcm) {
	// Converting stored passwords
	for (auto& file : std::filesystem::directory_iterator(pm->getStringData("dataloc"))) {
		std::string comm;													// To store comment
		std::string pass;													// To store password

		// Getting each data file name
		std::string fileName = file.path().string();

		// Opening file and backing up
		inpFile.open(fileName);
		std::getline(inpFile, comm);
		std::getline(inpFile, pass);
		inpFile.clear();
		inpFile.close();

		// Overwritting with new data
		outFile.open(fileName);
		outFile << newcm->inpEncrypt(oldcm->inpDecrypt(comm)) + "\n";
		outFile << newcm->inpEncrypt(oldcm->inpDecrypt(pass)) + "\n";
		outFile.clear();
		outFile.close();
	}

	// Converting stored reference names
	for (auto& file : std::filesystem::directory_iterator(pm->getStringData("refloc"))) {
		std::string refName;											// To store reference name

		// Getting each data file name
		std::string fileName = file.path().string();

		// Opening file and backing up
		inpFile.open(fileName);
		std::getline(inpFile, refName);
		inpFile.clear();
		inpFile.close();

		// Overwritting with new data
		outFile.open(fileName);
		outFile << newcm->inpEncrypt(oldcm->inpDecrypt(refName)) + "\n";
		outFile.clear();
		outFile.close();
	}
}

// Method to add a new password to the database
bool addPass(std::string refName) {
	std::string passCom;										// To store commments about passwrds
	std::string actPass;										// To store the actual password
	std::string conPass;										// To store the confirmation password
	std::string hashRefName;									// To store hashed refName

	// Hashing reference name
	hashRefName = std::to_string(std::hash<std::string>{}(refName));

	// Checking if duplicate refName
	if (std::filesystem::exists(pm->getStringData("dataloc") + hashRefName + ".pass") == true) {
		std::cout << "Reference name already exists, please enter another name" << std::endl;
		return false;
	}

	std::cout << "Comments : ";
	passCom = getInput();
	std::cout << "Enter password : ";
	actPass = getReqInput();
	std::cout << "Confirm password : ";
	conPass = getReqInput();

	// Checking if conPass == actPass
	if (conPass.compare(actPass) == 0) {
		// Adding data to database
		outFile.open(pm->getStringData("dataloc") + std::to_string(std::hash<std::string>{}(refName)) + ".pass");
		outFile << cm->inpEncrypt(passCom) << "\n";
		outFile << cm->inpEncrypt(actPass) << "\n";
		outFile.clear();
		outFile.close();

		// Adding refName data
		outFile.open(pm->getStringData("refloc") + hashRefName + ".ref");
		outFile << cm->inpEncrypt(refName) << "\n";
		outFile.clear();
		outFile.close();

		return true;
	}
	else {
		std::cout << "Confirmation failed ! Please try again !" << std::endl;
		return false;
	}
}

// Method to get encrypted pass from database
bool getPass(std::string refName) {
	std::string enPass;											// To store encrypted password
	std::string hashRefName;									// To store hashed refName
	
	// Hashing refName
	hashRefName = std::to_string(std::hash<std::string>{}(refName));

	// If reference name not found
	if (std::filesystem::exists(pm->getStringData("dataloc") + hashRefName + ".pass") == false) {
		// Exitting from program
		return false;
	}

	// Getting data from database
	inpFile.open(pm->getStringData("dataloc") + hashRefName + ".pass");
	std::getline(inpFile, enPass);
	enPass = "";
	std::getline(inpFile, enPass);
	inpFile.clear();
	inpFile.close();

	// Getting encrypted password
	enPass = enPass.substr(0, enPass.find(' '));
	
	// Decrypting password and displaying
	std::cout << cm->inpDecrypt(enPass);
	return true;
}

// Method to get the list of all passwords stored by pass-man and displaying only refName and comments
void getList() {
	std::string refName;											// To store file name
	std::string passCom;											// To store comment
	std::string hashRefName;										// To store hashed refName

	// Iterating through directory
	for (auto& file : std::filesystem::directory_iterator(pm->getStringData("refloc"))) {
		// Getting refName
		hashRefName = file.path().string().substr(pm->getStringData("refloc").length());
		hashRefName = hashRefName.substr(0, hashRefName.length() - 4);
		
		// Getting actual refName (encrypted)
		inpFile.open(pm->getStringData("refloc") + hashRefName + ".ref");
		std::getline(inpFile, refName);
		inpFile.clear();
		inpFile.close();

		// Decrypting refName
		refName = cm->inpDecrypt(refName);

		// Getting comment from file
		inpFile.open(pm->getStringData("dataloc") + hashRefName + ".pass");
		std::getline(inpFile, passCom);
		inpFile.clear();
		inpFile.close();

		// Displaying
		std::cout << "\nReference Name : " << refName << std::endl;
		std::cout << "Comments       : " << cm->inpDecrypt(passCom) << std::endl;
	}
}

// Method to modify a stored password using refName
bool editPass(std::string refName) {
	int oprChoice;											// To store operation choice
	std::string choice;
	std::string line1;										// To store the input from file (Comm)
	std::string line2;										// To store the input from file	(Pass)
	std::string newRefName;									// To store new reference name
	std::string newPassCom;									// To store new comment
	std::string newPass;									// To store new password
	std::string newPassCon;									// To store new password confirmation
	std::string hashRefName;								// To store hashed reference name

	// Hashing refName
	hashRefName = std::to_string(std::hash<std::string>{}(refName));

	// If refName exists
	if (std::filesystem::exists(pm->getStringData("dataloc") + hashRefName + ".pass") == true) {
		// Storing original line temporarily
		inpFile.open(pm->getStringData("dataloc") + hashRefName + ".pass");
		std::getline(inpFile, line1);
		std::getline(inpFile, line2);
		inpFile.clear();
		inpFile.close();
		
		// Asking for operation
		std::cout << "1. Change reference name" << std::endl;
		std::cout << "2. Change comment" << std::endl;
		std::cout << "3. Change password\n" << std::endl;
		
		try {
			// Taking choice
			std::getline(std::cin, choice);
			oprChoice = std::stoi(choice);
		}
		catch (std::invalid_argument ia) {
			std::cout << "Please enter number only !" << std::endl;
			return false;
		}

		switch (oprChoice) {
		// For changing reference name
		case 1: {
			std::cout << "Enter new reference name : ";
			newRefName = getInput();

			// Deleting old data
			std::filesystem::remove(pm->getStringData("dataloc") + hashRefName + ".pass");
			std::filesystem::remove(pm->getStringData("refloc") + hashRefName + ".ref");

			// Writing new data (data)
			outFile.open(pm->getStringData("dataloc") + std::to_string(std::hash<std::string>{}(newRefName)) + ".pass");
			outFile << line1 << "\n";
			outFile << line2 << "\n";
			outFile.clear();
			outFile.close();

			// Encrypted refName
			outFile.open(pm->getStringData("refloc") + std::to_string(std::hash<std::string>{}(newRefName)) + ".ref");
			outFile << cm->inpEncrypt(newRefName) << "\n";
			outFile.clear();
			outFile.close();

			return true;
		}
		// For changing comment
		case 2: {
			std::cout << "Enter new comment : ";
			newPassCom = getInput();

			// Deleting old data
			std::filesystem::remove(pm->getStringData("dataloc") + hashRefName + ".pass");

			// Writing new data
			outFile.open(pm->getStringData("dataloc") + hashRefName + ".pass");
			outFile << cm->inpEncrypt(newPassCom) + "\n";
			outFile << line2 + "\n";
			outFile.clear();
			outFile.close();
			return true;
		}
		// For changing password
		case 3: {
			std::cout << "Enter new password : ";
			newPass = getReqInput();
			std::cout << "Confirm new password : ";
			newPassCon = getReqInput();

			// Checking for confirmation
			if (newPass.compare(newPassCon) != 0) {
				std::cout << "Confirmation failed !" << std::endl;
				return false;
			}
			
			// Deleting old data
			std::filesystem::remove(pm->getStringData("dataloc") + refName + ".pass");

			// Writing new data
			outFile.open(pm->getStringData("dataloc") + refName + ".pass");
			outFile << line1 << "\n";
			outFile << cm->inpEncrypt(newPass) << "\n";
			outFile.clear();
			outFile.close();
			return true;
		}
		// For no match case
		default: {
			return false;
		}
		}
	}
	// If refName does not exist
	else {
		std::cout << "Reference name not found, password does not exist" << std::endl;
		return false;
	}
}

// Method to delete a stored password
bool delPass(std::string refName) {
	std::string delChoice;											// To store confirmation choice
	std::string hashRefName;										// To store hashed refName
	
	// Hashing refName
	hashRefName = std::to_string(std::hash<std::string>{}(refName));

	// Checking if reference name does not exist
	if (std::filesystem::exists(pm->getStringData("refloc") + hashRefName + ".ref") == false) {
		std::cout << "No such reference name found !" << std::endl;
		return false;
	}
	// If exists
	std::cout << "Are you sure you want to delete " << refName << "?(y/n) : ";
	std::getline(std::cin, delChoice);

	// If confirmed yes
	if (toLower(delChoice).compare("y") == 0) {
		// Deleting
		std::filesystem::remove(pm->getStringData("dataloc") + hashRefName + ".pass");
		std::filesystem::remove(pm->getStringData("refloc") + hashRefName + ".ref");
		return true;
	}
	// If no
	else {
		std::cout << "Operation cancelled" << std::endl;
		return false;
	}
}

// Method to backup passwords to specified location
bool backPass(std::string backLoc) {
	// If specified location does not exist
	if (std::filesystem::exists(backLoc) == false) {
		std::cout << "Specified location does not exist ! Please specify a different location" << std::endl;
		return false;
	}

	// Backing up data
	// Creating folder
	backLoc = backLoc + "\\Pass-Man_Backup";
	std::filesystem::create_directory(backLoc);

	// Copying data
	std::filesystem::copy(pm->getStringData("dataloc"), backLoc);
	std::filesystem::copy(pm->getStringData("refloc"), backLoc);

	return true;
}

// Method tp restore passwords from specific location
bool restorePass(std::string backLoc) {
	// If specified location does not exist
	if (std::filesystem::exists(backLoc) == false) {
		std::cout << "Specified backup location does not exist ! Please specify a different location" << std::endl;
		return false;
	}
	
	// Restoring data file by file
	for (auto& fileName : std::filesystem::directory_iterator(backLoc)) {
		// Trying to copy file
		try {
			std::filesystem::copy(std::filesystem::absolute(fileName.path()), pm->getStringData("dataloc"));
			std::filesystem::copy(std::filesystem::absolute(fileName.path()), pm->getStringData("refloc"));
		}
		// If file already exists
		catch (std::exception e) {
			// Skip over to next file if this file already exists
			continue;
		}
	}

	return true;
}

// Method to change/create new authorization key
bool authKey() {
	std::string newAuthKey;										// To store new authorization key
	// Checking if previous authorization key present
	if (std::filesystem::exists(pm->getStringData("aufile")) == true) {
		std::cout << "Authorization key has already been set up. Do you wish to change it ?(y/n) : ";
		std::string aChoice;
		std::getline(std::cin, aChoice);
		// If yes
		if (aChoice.compare("y") == 0) {
			// Deleting previous auth key
			std::filesystem::remove(pm->getStringData("aufile"));
			
			// Changing authorization key
			sm->changeAuthKey();			
			return true;
		}
		else {
			return false;
		}
	}
	else {
		std::cout << "No authorization key has been set up. Do you wish to create one now?(y/n) : ";
		std::string aChoice;
		std::getline(std::cin, aChoice);
		// If yes
		if (aChoice.compare("y") == 0) {
			if (sm->changeAuthKey() == true) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
}

// Method to set mail-id to send notifications
bool setEmail() {
	bool isMailSent;											// To check if mail is sent
	std::string mailId;											// To store mail-id entered by user
	std::string oneTimePass;									// To store one time password
	std::string inputOtp;										// To store otp entered by user
	std::string uChoice;										// To store user choice

	// Getting previous mail id
	// Opening current user registry and PassMan subkey
	Microsoft::Win32::RegistryKey^ regKey = Microsoft::Win32::Registry::CurrentUser->CreateSubKey(gcnew System::String("SOFTWARE\\PassMan"));

	// Getting mail-id if present else returning NA and storing as std::string
	mailId = msclr::interop::marshal_as<std::string>(regKey->GetValue(gcnew System::String("Mail-ID"), gcnew System::String("NA"))->ToString());

	// Checking if mail already set up
	// If does not set up
	if (mailId.compare("NA") == 0 || mailId.compare("") == 0) {
		std::cout << "Mail-id has not been set up. Do you want to set it now ?(y/n) : ";
	}
	// If exists
	else {
		std::cout << "Mail-id has already been set. Do you want to change it ?(y/n) : ";
	}
	getline(std::cin, uChoice);

	// If no
	if (toLower(uChoice).compare("y") != 0) {
		return false;
	}
																
	// Asking for mail id to send alert notifications
	std::cout << "Enter mail-id to send alert notifications to : ";
	std::getline(std::cin, mailId);

	// To remove mail-id
	if (mailId.compare("") == 0) {
		// Writing blank mail to registry
		regKey->SetValue(gcnew System::String("Mail-ID"), gcnew System::String(msclr::interop::marshal_as<System::String^>(mailId)), Microsoft::Win32::RegistryValueKind::String);

		// Closing connection to registry
		regKey->Close();
		
		std::cout << "Mail-ID removal successful !" << std::endl;
		return true;
	}

	// Generating otp of length 6
	oneTimePass = randomString(6);

	// Sending otp
	isMailSent = sendMail(mailId, "OTP for email id verification of pass-man", "Your OTP to verify your mail-id is : " + oneTimePass);

	// Checking status
	if (isMailSent == false) {
		std::cout << "Please enter a valid e-mail id !" << std::endl;
		return false;
	}
	else {
		// Asking for otp
		std::cout << "OTP has been sent to the given mail-id. Enter it to verify it's you : ";
	}

	// Checking OTP
	std::getline(std::cin, inputOtp);
	if (inputOtp.compare(oneTimePass) != 0) {
		std::cout << "Wrong OTP! Email verification failed !" << std::endl;
		return false;
	}

	std::cout << "Email-id verification complete !" << std::endl;
	
	// Storing email to regitstry
	regKey->SetValue(gcnew System::String("Mail-ID"), gcnew System::String(msclr::interop::marshal_as<System::String^>(mailId)), Microsoft::Win32::RegistryValueKind::String);
	
	// Closing connection to registry
	regKey->Close();

	return true;
}