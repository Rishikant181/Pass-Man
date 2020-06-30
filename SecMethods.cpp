// This files contains various methods related to security

#include "GlobalMembers.h"

// Using cli .net library
using namespace System::Net;

// Defining member method
secMan::secMan(std::string auFile) {
	// Initialising member data
	auFileName = auFile;
}

// Defining method getStringData
std::string secMan::getStringData(std::string data) {
	if (data.compare("authpass") == 0) {
		return authPass;
	}
}

// Defining method checkAuth
bool secMan::checkAuth() {
	std::string inpAuthPass;										// To store input auth pass
	std::string actAuthPass;										// To store actual auth pass
	
	// Taking input
	std::cout << "Enter authorization password : ";
	inpAuthPass = getReqInput();

	// Getting actual auth pass
	inpFile.open(auFileName);
	std::getline(inpFile, actAuthPass);
	inpFile.clear();
	inpFile.close();

	// Checking for correct auth
	// For successful authorization
	if (actAuthPass.compare(std::to_string(std::hash<std::string>{}(inpAuthPass))) == 0) {
		// Initialising authPass for later use
		authPass = inpAuthPass;
		return true;
	}
	else {
		return false;
	}
}

// Defining method changeAuthKey
bool secMan::changeAuthKey() {
	std::string newAuthKey;										// To store new auth key
	std::string conAuthKey;										// To store auth key confirmation
	std::string mailId;											// To store input mail id

	// Taking new auth key
	while (true) {
		std::cout << "Enter new authorization password (length 8-20 characters) : ";
		newAuthKey = getReqInput();
		if (newAuthKey.length() >= 8 && newAuthKey.length() <= 20) {
			break;
		}
		else {
			std::cout << "Please enter a valid passwprd !" << std::endl;
			continue;
		}
	}

	// Confirming auth key
	std::cout << "Confirm authorization password : ";
	conAuthKey = getReqInput();

	// Checking confirmation
	if (conAuthKey.compare(newAuthKey) != 0) {
		std::cout << "Confirmation failed !" << std::endl;
		return false;
	}

	// Storing new auth key hashed
	outFile.open(auFileName);
	outFile << std::hash<std::string>{}(newAuthKey) << "\n";
	outFile.clear();
	outFile.close();

	// Converting stored data
	// New object for new authPass
	cryptMan* ncm = new cryptMan(newAuthKey);

	convertData(cm, ncm);

	// Deleting unused pointer
	delete ncm;

	// Pointing dangling pointer to NULL
	ncm = NULL;

	return true;
}

// Defining method failAuthMail
bool secMan::failAuthMail() {
	// Getting mail id from registry
	// Opening current user registry and PassMan subkey
	Microsoft::Win32::RegistryKey^ regKey = Microsoft::Win32::Registry::CurrentUser->CreateSubKey(gcnew System::String("SOFTWARE\\PassMan"));

	// Getting mail-id if present else returning NA and storing as std::string
	std::string mailId = msclr::interop::marshal_as<std::string>(regKey->GetValue(gcnew System::String("Mail-ID"), gcnew System::String("NA"))->ToString());
	
	// Closing connection to registry
	regKey->Close();

	// Alerting user if mail-id set up
	if (mailId.compare("NA") != 0) {
		std::string timeString;                      // To store time as string
		time_t now = time(NULL);                     // To store time_t
		char nowTime[26];                            // To store current time

		// Getting current time
		ctime_s(nowTime, 26, &now);
		// Storing time as string
		timeString = nowTime;

		// Sending mail and storing status
		bool isMailSent = sendMail(mailId, "Unsuccessful access attempt of pass-man", "Someone tried to access your pass-man passwords at time : " + timeString);

		return isMailSent;
	}

	return false;
}