#include <iostream>
#include <string>
using namespace std;

// Program:		Distributed TigerBook - statusCache
// Name:			Samuel Skipper
// Class:			COMP 2710 Lab 2
// Date:				10/27/2017
// E-Mail:			sos0006@auburn.edu
// Description:	statusCache class handles all status that need to be
//						organized. This object is mostly used in an array
//						to sort messages by their timestamp ID
//						
//

class statusCache

{
private:

public:

	int statusId; //id for timestamp
	string rawMessage; //full message of statusCache
	
	//constructor without input, empty
	statusCache()
	{
		statusId = 0;
		rawMessage = "";
	};
	
	//constructor with input for id and message
	statusCache(int idIn, string msgIn)
	{
		setStatusCache(idIn, msgIn);
	};
	
	//setting id and message of instance
	void setStatusCache(int statusIn, string msgIn)
	{
		statusId = statusIn;
		rawMessage = msgIn;
	};
	
	//returns only id of instance
	int getId()
	{
		return statusId;
	};
	
	//returns full message
	string getMsg()
	{
		return rawMessage;
	}

};