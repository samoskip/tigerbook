#include <iostream>
#include <stdlib.h>
#include <string>
#include "systemObj.h"
using namespace std;

// Program:		Distributed TigerBook - menu
// Name:			Samuel Skipper
// Class:			COMP 2710 Lab 2
// Date:				10/27/2017
// E-Mail:			sos0006@auburn.edu
// Description:	Menu and main class for TigerBook
//						This handles all the menu operations
//						Giving several options for users to create
//						users and share messages
//

void inputMessage(string nameOfGroup); //function that handles multi line messages
void inputUnicastMessage(string nameOfGroup); //function that handles multi line messages


char menuNav; //char for navigating menu
bool terminateMsg = true; // ends loop when "!^" is given
bool userLoggedIn = false; //for continuing menu loop
string messageA; //temp message for building actual msg for buffer
string usernameIn; //input for creating username
string groupnameIn; //input for creating group
string menuInput; //char for navigating menu
string tempGroup; //temp variable for setting up group messages
systemObj m; //obj for using systemObj header
string border = "===========================================================";

int main()
{
	//Welcome message
	cout << border + "\n";
	cout << "|             Distributed TigerBook System!            |\n";
	cout << border + "\n\n";	
	
	//Endless loop for menu and program, ends when user exits
	while (true)
	{
		
		/* 
		/Create a new user. Prompts the user of a username,
		/and stores it in the userbase object
		*/
		if (!(userLoggedIn))
		{
			//Prompts user for name, stores in usernameIn variable
			cout << "Please enter user name: ";
			getline(cin, usernameIn);
			
			//Checking for duplicates, if none are found, user is added
			if (m.checkDuplicates(usernameIn))
			{
				m.addNewUser(usernameIn);
			}
			
			//If username is a duplicate, give error and return to menu
			else
			{
				m.setCurrentUser(m.findUser(usernameIn));
			}
			
			cout << endl << border << "\n";
			cout << "|    Welcome to Distributed TigerBook System, ";
			cout << m.currentUsername() << "!  |\n";
			cout << border + "\n\n";	
			userLoggedIn = true;
			
			m.loadUserGroups(m.currentUsername());
		}
		
		//Giving menu options, this is repeated after every action
		cout << "Broadcast (b), Multicast (m), ";
		cout << "Unicast (u), Wall (w), Home (h),\n";
		cout << "Create group (g), Join group (j), Quit (q)\n";
		cout << endl;
		cout << "Choose an option: ";
		
		//Prompting for menu input
		getline(cin, menuInput); //prompting for menu imput
		cout << endl;
		menuNav = menuInput[0]; //converting string input to char
		menuInput = ""; //clearing menu so whitespace doesn't carry over
		
		//Begin menu options, accepts lower and upper case
		switch(menuNav) 
		{
			
			/* 
			/Broadcast a message, creates a message that all users will see
			/Message is stored in #All group
			*/
			case 'b' : case 'B' :
			
				//Prompts user to enter message, stores input in buffer
				cout << "Enter message: ";
				m.bufferHeader(m.currentUsername());
				inputMessage("_All");
				cout << endl;
				cout << border + "\n";
				cout << "|        " << m.currentUsername();
				cout << " has broadcasted a new message        |\n";
				cout << border + "\n\n";
				break;
				
			/* 
			/Sends a message to a group
			/Message is stored in user defined group
			*/
			case 'm' : case 'M' :
				
				//Prompts for user to input group
				cout << "Please enter the group name: ";
				getline(cin, groupnameIn);
				if (!(m.checkGroupDupes(groupnameIn)))
				{
					//Defining header for message buffer
					m.bufferHeader(m.currentUsername());
					cout << "\nEnter message: ";
					inputMessage(groupnameIn);
					cout << endl;
					cout << endl;
					cout << border + "\n";
					cout << "|     " << m.currentUsername();
					cout << " has multicasted a message to group ";
					cout << groupnameIn << "     |\n";
					cout << border + "\n\n";
				}
				
				//Gives error if group is not defined
				else
				{
					cout << "\nGroup not found, try again.\n\n";
				}

				break;
				
			/* 
			/Sends a message to a individual
			/The group section in the header will be the recipient's name
			*/
			case 'u' : case 'U' :
			
				//Prompts for who this message is being sent to
				cout << "Enter the recipient name: ";
				getline(cin, usernameIn);
				
				//Making sure the recipient exists
 				if (!(m.checkDuplicates(usernameIn)))
				{ 
					m.bufferHeader(m.currentUsername());
					cout << "\nEnter message: ";
					inputUnicastMessage(usernameIn);
					cout << endl;
					cout << border + "\n";
					cout << "|         " << m.currentUsername();
					cout << " has unicasted a message to ";
					cout << usernameIn << "             |\n";
					cout << border + "\n\n";
 				}
				
				//If recipient doesn't exist, error
				else
				{
					cout << "\nRecipient not found, try again.\n\n";
				} 

				break;
			
			/* 
			/Goes to users wall page, this is their outbox
			/Will initially display first two messages before progressing
			*/
			case 'w' : case 'W' :
			
				//Displays user's wall page
				cout << border + "\n";
				cout << "|                 " << m.currentUsername();
				cout << "'s Wall Page                |\n";
				cout << border + "\n\n";
				cout << m.printWall(m.currentUsername());
				cout << border + "\n";
				cout << "|              End of " << m.currentUsername();
				cout << "'s Wall Page               |\n";
				cout << border + "\n\n";
				
				break;
				
			/* 
			/Goes to users home page, this is their inbox
			/Will initially display first two messages before progressing
			*/
			case 'h' : case 'H' :

				//Display user's home page
				cout << border + "\n";
				cout << "|                 " << m.currentUsername();
				cout << "'s Home Page                |\n";
				cout << border + "\n\n";
				cout << m.printHome(m.currentUsername());
				cout << border + "\n";
				cout << "|              End of " << m.currentUsername();
				cout << "'s Home Page              |\n";
				cout << border + "\n\n";

				break;
				
				
			/* 
			/Prompts user for group name for creating new group
			/New groups cannot share names with other groups
			*/
			case 'g' : case 'G' :	
			
				//Prompting for new group name
				cout << "Please enter the group name: ";
				getline(cin, groupnameIn);
				cout << endl;
				
				//Makes sure new group name starts with hashtag
				if (groupnameIn[0] != '#')
				{
					cout << "Group names MUST start with a hashtag ";
					cout << "symbol.\nE.g. #Tigers\n\n";
				}
				
				//Checking for duplicates, if none found progress
				else if (m.checkGroupDupes(groupnameIn))
				{
					m.addNewGroup(groupnameIn);
					cout << border + "\n";
					cout << "|            New group " << groupnameIn;
					cout << " group created               |\n";
					cout << border + "\n\n";	
				}
				
				//Gives error if group is a duplicate
				else
				{
					cout << "Group has already been created.\n\n";
				}
				
				break;
				
			/* 
			/Prompts user for group name for joining group
			/Groups must have been previously created
			*/
			case 'j' : case 'J' :
			
				//Prompts user for which group name to join
				cout << "Please enter the group name: ";
				getline(cin, groupnameIn);
				cout << endl;
				
				//If user joins group he's already involed with
				if ((groupnameIn == "#All")
							|| (m.isUserInGroup(groupnameIn)))
				{
					cout << "You're already in this group.\n\n";
				}
				
				//If group exists, user joins group
				else if (!(m.checkGroupDupes(groupnameIn)))
				{
					m.joinGroup(groupnameIn, true);
					cout << border + "\n";
					cout << "|            " << m.currentUsername();
					cout << " has joined group " << groupnameIn;
					cout << "              |\n";
					cout << border + "\n\n";
				}
				
				//Otherwise, group must not exist, error
				else
				{
					cout << "Group does not exist. Try again.\n\n";
				}
				
				break;				

			/* 
			/Exits the program
			/No infomation will be saved
			*/
			case 'q' : case 'Q' :
			
				cout << border + "\n";
				cout << "|  Thank you for using the Distributed TigerBook System";
				cout << "  |\n";
				cout << border + "\n";	
				exit(0);
			
			/* 
			/Assumes every other input is unintentional
			/Gives error than continues loop
			*/
			default :
			
				cout << "Error, option not recognized. Try again.\n\n";
		}
	
		//resetting values
		usernameIn = "";
		groupnameIn = "";
	}
	
	return 0;
}

/* 
/Adds each input line to the buffer
/Continues indefinitely until "^!" is entered
*/
void inputMessage(string nameOfGroup)
{
	while(terminateMsg) 
	{
		getline(cin, messageA);
		m.addToBuffer(messageA);
			
		// terminates loop when "^!" is entered
		if (messageA == "^!")
		{
			terminateMsg = false;
			messageA = "";
			m.compileBuffer(nameOfGroup);
		}
	}
	terminateMsg = true;
}

/* 
/Adds each input line to the buffer
/Continues indefinitely until "^!" is entered
/Taylored for unicast input
*/
void inputUnicastMessage(string nameOfGroup)
{
	while(terminateMsg) 
	{
		getline(cin, messageA);
		m.addToBuffer(messageA);
			
		// terminates loop when "^!" is entered
		if (messageA == "^!")
		{
			terminateMsg = false;
			messageA = "";
			m.compileUnicastBuffer(nameOfGroup);
		}
	}
	terminateMsg = true;
}