#include <iostream>
#include <string>
using namespace std;

// Program:		Distributed TigerBook - userbase
// Name:			Samuel Skipper
// Class:			COMP 2710 Lab 2
// Date:				10/27/2017
// E-Mail:			sos0006@auburn.edu
// Description:	Userbase class handles all user defined functions
//						Usernames, groups joined, among other details
//						are defined here
//						
//

class userbase

{
private:

public:

	int numOfGroupsJoined; //Number of groups user has joined
	string username; //The actual name of user
	string myGroups[32]; //Array of groups user has joined
	
	//Constructor with no parameters
	userbase()
	{
		numOfGroupsJoined = 0;
	};
	
	//Constructor with username as parameter
	userbase(string newUser)
	{
		numOfGroupsJoined = 0;
		setUser(newUser);
	};
	
	//Return name of user
	string getUser()
	{
		return username;
	};
	
	//Set user to new name
	void setUser(string newUser)
	{
		username = newUser;
	};
	
	//Add group to users group array
	void joiningGroup(string groupName)
	{
		myGroups[numOfGroupsJoined] = groupName;
		numOfGroupsJoined++;
	};
	
	//Print a list of groups user has joined
	string printGroupsJoined()
	{
		int i = 0;
		string toStringGroups = "";
		while ((i < numOfGroupsJoined))
		{
			toStringGroups = toStringGroups + " " + myGroups[i];
			i++;
		}
		return toStringGroups;
	};
	
	//Individually print groups user has joined
	string printIndividualGroups(int num)
	{
		return myGroups[num];
	};
	
	//Return int of amount of groups user has joined
	int printAmountOfGroups()
	{
		return numOfGroupsJoined;
	};
};