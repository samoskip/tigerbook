#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "userbase.h"
#include "groupbase.h"
#include "msgBuffer.h"
#include "statusCache.h"
using namespace std;

// Program:		Distributed TigerBook - systemObj
// Name:			Samuel Skipper
// Class:			COMP 2710 Lab 2
// Date:				10/27/2017
// E-Mail:			sos0006@auburn.edu
// Description:	This is a system for instantiating user, groups, and
//						message buffer objects. SystemObj handles much
//						of the back end for TigerBook
//						
//

//Set up banners for every actions

class systemObj

{
private:

public:

	static const int DATA_LIMIT = 128; //amount of arrays created

	int numberOfMsg; //Keeping tabs on the amount of message in buffer
	int userSize; //Keeping tabs on size of users for finding
	int groupSize; //Keeping tabs on group size for finding
	
	userbase userDB[DATA_LIMIT]; //Setting up array for database of users
	groupbase groupDB[DATA_LIMIT]; //Setting up array for database of groups
	
	userbase current; //current user that is being dealt with
	groupbase crtGroup; //current group that is being dealt with
	msgBuffer g; //msgBuffer header for buffer functions
	statusCache si; //instance of statusCache for holding messages
	
	ifstream inStream; //inStream for txtfile input
	ofstream outStream; //outStream for txtfile output
	
	//Constructor setting some default values
	systemObj()
	{
		userSize = 0;
		groupSize = 0;
		loadGroups(); //loading in groupdb file
		loadUserData(); //loading in userdb file
		
		inStream.open("num.txt"); 
		inStream >> numberOfMsg; //loading amount of users
		inStream.close();
	};
	
	/* Loading text data functions
		The following are all for loading the textfiles containing users,
		groups, and other misc infomation */
	
	// load all groups in textfile to database
	void loadGroups()
	{
		string line;
		inStream.open("groupdb.txt");
		while(getline(inStream, line))
		{
			groupDB[groupSize] = groupbase(line);
			groupSize++;
		}
		inStream.close();
	};
	
	// load all users in textfile to database
	void loadUserData()
	{
		string line;
		inStream.open("userdb.txt");
		while(getline(inStream, line))
		{
			userDB[userSize] = userbase(line);
			userSize++;
		}
		inStream.close();
	};
	
	// load each users file that contains groups they are members of
	void loadUserGroups(string userName)
	{
		string line;
		string outFilename = (userName + "_groups.txt");
		inStream.open(outFilename.c_str());
		while(getline(inStream, line))
		{
			joinGroup(line, false);
		}
		inStream.close();
	};
	
	/* Userbase functions
		The following are all for creating, finding, and keeping tabs
		On current user */
		
	//Adds user to database, users cannot be deleted
	void addNewUser(string userInput)
	{
		userDB[userSize] = userbase(userInput);
		setCurrentUser(userDB[userSize]);
		userSize++;
		
		//adds user to database textfile
		outStream.open("userdb.txt", ios::app);
		outStream << userInput;
		outStream << endl;
		outStream.close();
	};
	
	//Return the user name of current user
	string currentUsername()
	{
		return current.getUser();
	};
	
	//Setting the username of an already existing user
	void setCurrentUser(userbase newName)
	{
		current = newName;
	};
	
	//Searching array for user, returns the user object if found
	userbase findUser(string userInput)
	{
		int i = 0;
		while ((i < userSize) && (userDB[i].getUser() != userInput))
		{
			i++;
		}
		if (i < userSize)
			return userDB[i];
		else
			return current;
	};
	
	//Searching array for duplicate users, returns true if none
	bool checkDuplicates(string userInput)
	{
		int i = 0;
		while ((i < userSize) && (userDB[i].getUser() != userInput))
		{
			i++;
		}
		if (i < userSize)
			return false;
		else
			return true;
	};
	
	//Returns the amount of users defined
	int amountOfUsers()
	{
		return userSize;
	};
	
	//Sets a user as joined in a group
	void joinGroup(string groupToJoin, bool writeToText)
	{
		current.joiningGroup(groupToJoin);
		
		// If true, write to users group file the name of group
		if (writeToText == true)
		{	
			string outFilename = (currentUsername() + "_groups.txt");
			outStream.open(outFilename.c_str(), ios::app);
			outStream << groupToJoin;
			outStream << endl;
			outStream.close();
		}
	};
	
	
	//Checks to see if user is involed with group
	bool isUserInGroup(string groupname)
	{
		int i = 0;
		while (i < current.printAmountOfGroups())
		{
			if (current.printIndividualGroups(i) == groupname)
			{
				return true;
			}
			i++;
		}
		return false;
	};
	
	/* Groupbase functions
		The following are all for creating, finding, and keeping tabs
		On groups */
		
	//Adds a new group object
	void addNewGroup(string userInput)
	{
		//formatting group name for creating textfile
		int titleBlock = userInput.size();
		string outFilename = ("_" + userInput.substr(1, titleBlock) + ".txt");
		outStream.open(outFilename.c_str(), ios::app);
		outStream.close();
		
		//adds group to array
		groupDB[groupSize] = groupbase(userInput);
		setCurrentGroup(groupDB[groupSize]);
		groupSize++;
		
		//adds group to database textfile
		outStream.open("groupdb.txt", ios::app);
		outStream << userInput;
		outStream << endl;
		outStream.close();
	};
	
	// adds message to username textfile
	void addNewUnicast(string userInput)
	{
		string outFilename = (userInput + ".txt");
		outStream.open(outFilename.c_str(), ios::app);
		outStream.close();
	};
	
	// compiles and outputs unicast message to textfile
	void compileUnicastBuffer(string nameOfGroup)
	{
		string outFilename = (nameOfGroup);
		g.bufferDone(outFilename);
	};
	
	//Changes group object to parameter's group
	void setCurrentGroup(groupbase newName)
	{
		crtGroup = newName;
	};
	
	//Checks group for duplicates, returns true if none are found
	bool checkGroupDupes(string userInput)
	{
		int i = 0;
		
		//search until index group database equals group name from user
		while ((i < groupSize) && (groupDB[i].getGroup() != userInput))
		{
			i++;
		}
		if (i < groupSize)
			return false;
		else
			return true;
	};
	
	/* Buffer functions
		For returning, adding, and formatting
		MessageBuffer properly */

	//Adds a line to the buffer and calls for a new line
	void addToBuffer(string newMsg)
	{
		g.bufferAdd(newMsg + "\n");
	};
	
	//Compiles the cache in the buffer into the raw message buffer
	void compileBuffer(string nameOfGroup)
	{
		int titleBlock = nameOfGroup.size();
		string outFilename = ("_" + nameOfGroup.substr(1, titleBlock));
		g.bufferDone(outFilename);
	};
	
	// formats input from "#group" to "_group.txt"
	string getGroupTextName(string nameOfGroup)
	{
		int titleBlock = nameOfGroup.size();
		string outFilename = ("_" + nameOfGroup.substr(1, titleBlock) + ".txt");

		return outFilename;
	};
	
	//Sets the header of the buffer as so, {!TIME!}<!NAME!>
	void bufferHeader(string userName)
	{
		// getting proper header format
		g.headerHold(numberOfMsg, userName);
		
		//updating number of messages
		inStream.open("num.txt");
		outStream.open("num.txt");
		inStream >> numberOfMsg;
		numberOfMsg++;
		outStream << numberOfMsg; 
		inStream.close();
		outStream.close();
	};
	
	//Returns the raw message buffer
	string printBuffer()
	{
		return g.getBuffer();
	};
	
	// finding all homepage messages and storing in array
	string printHome(string homeUser)
	{
		
		int i = 0; //Integer for messageHold array, amount of messages
		int k = 0; //Compares to amount of entires in group for exiting loop
		bool allScan = true; //Ensures that the #All group has been scanned
		bool dmScan = true; //True until user txt file have been scanned
		statusCache messageHold[DATA_LIMIT]; //Stores messages to be sorted
		string groupNames; //For holding name of groups for comparison
		groupbase grp; //For accessing groupbase
		
		// while groups, all, or DMs are to be scanned
		while ((k < groupSize) || (allScan) || (dmScan))
		{
			
			grp = groupDB[k];
			if ((isUserInGroup(grp.getGroup())) || (allScan) || (dmScan))
			{	
				int bufferStart; // keeps track for find function
				int bufferEnd; // keeps track for find function
				int bufferValue; //used for tracking how many characters in msg
				int endOfPrevMsg = 0; // keeping track of end of msg
				int id; // getting timestamp from message header
				bool moreLines = true; // more lines from text to scan
				string bufferCache; //storing complete message
				string msgFromText = ""; //storing entire content of grouptxt
				string line; // for building grouptxt line by line
		
				//if #All hasn't scanned, set group to #All
				if (allScan)
					groupNames = "#All";
				
				//sets group name to username for DMs
				else if (dmScan)
					groupNames = homeUser;
				
				//otherwise, go through all groups
				else
					groupNames = grp.getGroup();
				
				//open ifstream of group textfile
				string outFilename = (homeUser + ".txt");
				
				//if there are still DMs to scan, open username file
				if ((dmScan) && (allScan == false))
					inStream.open(outFilename.c_str());
				//otherwise scan group text files
				else
					inStream.open(getGroupTextName(groupNames).c_str());
				
				//while there is still text in file, add to string
				while(getline(inStream, line))
				{
					//reading txt file line by line
					msgFromText = msgFromText + "\n" + line;
				}
				inStream.close();
				
				// if #All textfile empty, confirm #All has been scanned, continue
				if ((msgFromText.size() == 0) && (allScan))
				{
					allScan = false;
					continue;
				}
				
				// if #All textfile empty, confirm #All has been scanned, continue
				else if ((msgFromText.size() == 0) && (dmScan))
				{
					dmScan = false;
					continue;
				}
				
				//After first round, begin searching every other group
				else if (msgFromText.size() == 0)
				{
					k++;
					continue;
				}
				
				//while there is still text in the file to scan
				while(moreLines)
				{
					// finding and id number
					bufferStart = (msgFromText.find("{!", endOfPrevMsg) + 2);
					bufferEnd = msgFromText.find("!}", endOfPrevMsg);
					bufferValue = (bufferEnd - bufferStart);
					
					// storing timestamp from message
					istringstream ( msgFromText.substr(bufferStart, bufferValue) ) >> id;

					// finding and printing the user name
					bufferStart = (msgFromText.find("<!", endOfPrevMsg)) + 2;
					bufferEnd = msgFromText.find("!>", endOfPrevMsg);
					bufferValue = (bufferEnd - bufferStart);
					
					// setting string to formatted header of message
					bufferCache = msgFromText.substr(bufferStart, bufferValue)
							+ " (" + groupNames + ") >\n";

					// finding and printing the message
					bufferStart = (msgFromText.find("!>", endOfPrevMsg) +2);
					bufferEnd = msgFromText.find("^!", endOfPrevMsg);
					bufferValue = (bufferEnd - bufferStart);		
					
					// finishing bufferCache to entire formatted message
					bufferCache = bufferCache + 
							msgFromText.substr(bufferStart, bufferValue) + "\n";
					
					// calibrating where message ends
					endOfPrevMsg = bufferEnd + 2;
					
					// if end of message, finish loop
					if (endOfPrevMsg == msgFromText.size())
					{
						moreLines = false;
					}
					
					// add message to array
					messageHold[i] = statusCache(id, bufferCache);
					i++;
					
					// continue loop until no more messages remain
					bufferCache = "";
				}
			}
			// ensure #All group has been scanned
			if (allScan)
				allScan = false;
				
			// if DMs have just been scanned
			else if ((allScan == false) && (dmScan))
				dmScan = false;
			
			//increment k, amount of groups scanned
			else
				k++;
		}
		
		return compileTextBuffer(i, messageHold);
		
	};
	
	// finding all wall messages and storing in array
	string printWall(string homeUser)
	{
		int i = 0; //Integer for messageHold array, amount of messages
		int k = 0; //Compares to amount of entires in group for exiting loop
		int j = 0; //Keeping track of users scanned for DMs
		bool allScan = true; //Ensures that the #All group has been scanned
		statusCache messageHold[DATA_LIMIT]; //Stores messages to be sorted
		string groupNames; //For holding name of groups for comparison
		groupbase grp; //For accessing groupbase
		userbase temp; //instance of user for scanning for DMs
		string dmText = ""; //To hold DM message
		bool firstPass = false; //After first round of loop, DMs are scanned
		
		//While there are still groups, All, or DM to scan
		while ((k < groupSize) || (allScan) || (j < userSize))
		{
			int bufferStart; // keeps track for find function
			int bufferEnd; // keeps track for find function
			int bufferValue; // used for tracking how many characters in msg
			int endOfPrevMsg = 0; // keeps track of where message ends
			int id; // for storing the message timestamp number
			bool moreLines = true; // while there are still lines to scan in textfile
			
			string userId = ""; // for storing the name of user of message
			string bufferCache; //Where message will be stored
			string msgFromText = ""; // for storing message from text
			string line; // for storing line from text
			
			//k increments, so this searches all of the database
			grp = groupDB[k];

			//If there are still users to scan for DMs
			if (j < userSize)
			{
				temp = userDB[j];
				groupNames = temp.getUser();
				dmText = (temp.getUser() + ".txt");
				inStream.open(dmText.c_str());
				j++;
			}
			
			//Otherwise, scan #All messages
			else if ((allScan) && (j == userSize))
			{
				groupNames = "#All";
				inStream.open(getGroupTextName(groupNames).c_str()); 
			}
			
			//Afterwards, scan every group
			else
			{
				groupNames = grp.getGroup();
				inStream.open(getGroupTextName(groupNames).c_str()); 
			}

			while(getline(inStream, line))
			{
				//reading textfile line by line
				msgFromText = msgFromText + "\n" + line;
			}
			inStream.close();

			// if all DMs scanned, stop scanning DMs and skip to next loop
			if ((msgFromText.size() == 0) && (allScan) && (firstPass == false)
					&& (j == userSize))
			{
				firstPass = true;
				continue;
			}
			
			// if DM is empty but still more to scan, skip to next loop
			else if ((msgFromText.size() == 0) && (allScan) &&
					(firstPass == false))
				continue;
			
			// if All is empty, skip to next loop
			else if ((msgFromText.size() == 0) && (allScan) && (firstPass))
			{
				allScan = false;
				continue;
			}
			
			// if any other textfile empty, increment k, skip to next loop
			else if ((msgFromText.size() == 0)
						&& (allScan == false) && (firstPass))
			{
				k++;
				continue;
			}

			// while there are still lines to scan
			while(moreLines)
			{
				// finding and id number
				bufferStart = (msgFromText.find("{!", endOfPrevMsg) + 2);
				bufferEnd = msgFromText.find("!}", endOfPrevMsg);
				bufferValue = (bufferEnd - bufferStart);
				
				// storing timestamp from message
				istringstream ( msgFromText.substr(bufferStart, bufferValue) ) >> id;

				// finding and printing the user name
				bufferStart = (msgFromText.find("<!", endOfPrevMsg)) + 2;
				bufferEnd = msgFromText.find("!>", endOfPrevMsg);
				bufferValue = (bufferEnd - bufferStart);
				
				// formatting header of message
				bufferCache = msgFromText.substr(bufferStart, bufferValue)
						+ " (" + groupNames + ") >\n";
				
				// storing the username of message
				userId = msgFromText.substr(bufferStart, bufferValue);
				
				// finding and printing the message
				bufferStart = (msgFromText.find("!>", endOfPrevMsg) +2);
				bufferEnd = msgFromText.find("^!", endOfPrevMsg);
				bufferValue = (bufferEnd - bufferStart);		
				
				// finish building and formatting message
				bufferCache = bufferCache + 
						msgFromText.substr(bufferStart, bufferValue) + "\n";
				
				//calibrating where the end of the message is
				endOfPrevMsg = bufferEnd + 2;

				// if the end of the message is false, finish while loop
				if (endOfPrevMsg == msgFromText.size())
					moreLines = false;

				// if message is from user, add to array
				if (userId == homeUser)
				{
					messageHold[i] = statusCache(id, bufferCache);
					i++; 
				}

				// continue loop until no more messages remain
				bufferCache = "";
			}
			
			// ensure #All group has been scanned
			if ((allScan) && (firstPass))
				allScan = false;
			
			//increment k, amount of groups scanned
			else if ((allScan == false) && (firstPass))
				k++;
			
			//confirms the pass scanning for all DMs is done
			if (j == userSize)
				firstPass = true;
			
		}
		// send array to function to be ordered correctly
		return compileTextBuffer(i, messageHold);
	};
	
	// function for putting messages from wall and home in proper order
	string compileTextBuffer(int n, statusCache msgAray [ ] )
	{
		int tempN = n; //function for restarting n
		int i = 0; //for keeping track of when two messages are shown
		int numTxt = numberOfMsg - 1; //tracking number of msg
		int msgLeft = n; //
		statusCache s; //instance for getting ID of each message
		
		// while there are still messages to compare
		while (numTxt >= 0)
		{
			//while still more of array to scan
			while (n >= 0)
			{
				//setting instance of statusCache to each elemet
				s = msgAray[n];
				
				// if id finds its proper place, not including empty array
				if ((s.getId() == numTxt) && (s.getMsg().size() > 0))
				{
					// printing message, newest first
					cout << s.getMsg();
					msgLeft--;
					
					// when two messages are shown with more ahead
					while ((i == 1) && (msgLeft > 0))
					{
						string moreMsgPromt; // for input for user
						
						//Ask user if they want two or all messages
						cout << "               More messages? (yes/no): ";
						getline(cin, moreMsgPromt);
						cout << endl;
						
						//If yes, show all messages
						if ((moreMsgPromt == "yes")
									|| (moreMsgPromt == "Yes"))
							break;
						
						//If no, return out of function
						else if ((moreMsgPromt == "no")
									|| (moreMsgPromt == "No"))
						{
							numTxt = 0;
							n = 0;
							return "";
						}
						
						//Otherwise, restart the loop and ask again
						else
						{
							cout << "Answer not recognized, ";
							cout << "answer yes or no\n\n";
						}
					}
					
					i++;

				}
				n--;
			}
			// restarting n
			n = tempN;
			numTxt--;
		}
		return "";
	};
};