// This file contains various methods and data for sending mails

#include "GlobalMembers.h"

using namespace System::Net;

// Method to send a mail to a specified email id and specific data
bool sendMail(std::string toAddress, std::string mailHeading, std::string mailBody) {
	Mail::SmtpClient^ smtpClient = gcnew Mail::SmtpClient();		// To store the smtp client
	
	// Converting std::string to System::String
	System::String^ toAdd = gcnew System::String(toAddress.c_str());
	System::String^ mHead = gcnew System::String(mailHeading.c_str());
	System::String^ mBody = gcnew System::String(mailBody.c_str());

	// Initialising
	smtpClient->EnableSsl = true;
	smtpClient->Host = "smtp.gmail.com";
	smtpClient->Port = 587;
	smtpClient->UseDefaultCredentials = false;
	smtpClient->Credentials = gcnew NetworkCredential("secur.passman@gmail.com", "Gabbar181Passman");

	// Sending mail
	try {
		smtpClient->Send("secur.passman@gmail.com", toAdd, mHead, mBody);
		return true;
	}
	catch (std::exception e) {
		std::cout << "Failed to send mail" << std::endl;
		return false;
	}	
}