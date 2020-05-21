#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

// Program:		Distributed TigerBook - msgBuffer
// Name:			Samuel Skipper
// Class:			COMP 2710 Lab 2
// Date:				10/27/2017
// E-Mail:			sos0006@auburn.edu
// Description:	MessageBuffer class handles the
//						message buffer string
//						Handles proper formatting and storing
//						of all messages from TigerBook
//

class msgBuffer

{
private:

public:

	string fullMsgBuffer; //The entire raw message buffer
	string headerCache; //Cache for the header with user and group name
	string msgCache; //Message cache for building message
	ifstream inFile; //ifstream for input from text
	ofstream outFile; //outstream for output from text
	
	//Message buffer defining variables
	msgBuffer()
	{
		fullMsgBuffer = "";
		msgCache = "";
	};
	
	//Adding messages to cache
	void bufferAdd(string newMsg)
	{
		msgCache = msgCache + newMsg;
	};
	
	//Adding header to cache
	void headerHold(int msgNum, string header)
	{
		stringstream ss;
		ss << msgNum;
		string str = ss.str();
		headerCache = "{!" + str +"!}<!" + header + "!>";
	};
	
	//Builds the cache into the buffer
	void bufferDone(string groupFile)
	{
		string grpFile = groupFile + ".txt";
		outFile.open(grpFile.c_str(), ios::app);
		fullMsgBuffer = headerCache + msgCache + fullMsgBuffer;
		headerCache = "";
		msgCache = "";
		outFile << fullMsgBuffer;
		emptyBuffer();
		outFile.close();
	};
	
	//Returns the full raw message buffer
	string getBuffer()
	{
		return fullMsgBuffer;
	};
	
	//Clears buffer, resets
	void emptyBuffer()
	{
		fullMsgBuffer = "";
	};

};