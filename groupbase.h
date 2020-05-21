#include <iostream>
#include <string>
using namespace std;

// Program:		Distributed TigerBook - groupbase
// Name:			Samuel Skipper
// Class:			COMP 2710 Lab 2
// Date:				10/27/2017
// E-Mail:			sos0006@auburn.edu
// Description:	Groupbase class handles all group defined functions
//						Name of groups are defined here. Groupbase are
//						tied with userbase for users who have joined
//						certain groups
//

class groupbase

{
private:

public:
	
	string groupname; //Actual name of group
	
	//Constructor without parameters
	groupbase()
	{
	};
	
	//Constructor with name of group has parameter
	groupbase(string newGroup)
	{
		setGroup(newGroup);
	};
	
	//Return name of group
	string getGroup()
	{
		return groupname;
	};
	
	//Set group name
	void setGroup(string newGroup)
	{
		groupname = newGroup;
	};
	
	
};