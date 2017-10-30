// LCR.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <iterator>
#include <chrono>
#include <windows.h>
#include <fstream>

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------Player Class------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////
class Player {

	private:
		int chips; //the number of chips a particular player has at any given moment
		bool activePlayer; //not used
		string name;

	//constructor
	public:
		Player(string name_, int chips_, bool activePlayer_) : name(name_), chips(chips_), activePlayer(activePlayer_) {}
	
		bool getActivePlayer() {
			return activePlayer;
		}

		void setActivePlayer(bool activePlayer_) {
			activePlayer = activePlayer_;
		}
		
		int getChips() const {
			return chips;
		}

		void setChips(int chips_) {
			chips = chips_;		
		}

		void subtractChips() {
			chips--;
		}

		void addChips() {
			chips++;
		}

		string getName() const  {
			return name;
		}

		void setName(string *name_) {
			name = *name_;
		}

		//the function below is made so a player can role the dice. The function returns a vector that contains the dice roled based on the number of chips they have.
		//if they have more than three they will only get three char in the vector, if they have less, then the vector will only reuturn that number of roles that is equal to the the chips they have
		vector<char> roleDice() {
			int count = 0; //the count is used to figure out a player has 3 or less chips
			vector<char> allDice; //a vector that is returned with the results
			int randomNumber;
			vector<char> move; //create a vector to store the char LCRN
			srand(time(0));

			//create each char in the vector
			move.push_back('L');
			move.push_back('C');
			move.push_back('R');
			move.push_back('N');

			while ((count < chips) && !(count >= 3)) {
				//generate number from 1 to 4
				randomNumber = rand() % 4 + 1;
				Sleep(300); //this delay time is to ensure that there is enough time for the random number to be truly random
				allDice.push_back(move[randomNumber - 1]);
				count++;	
			}
			return	allDice; //returns a vector that contians the random chars, LCR or N for neutral--do nothing that is.
		}
};
//-------------------------------end of Player class-------------------------------------------
//---------------------------------------------------------------------------------------------


///////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------Environment class----------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////
class Environment {

	private:
		int randNumber; //not used
		int numberOfChipsPerPlayer; //stores how many chips each player is getting
		int numberOfChipsInCenter; //counts the chips placed in the center
		vector<Player> players; //creates a vector of players.

	//constructor; it takes the palerys vector by address and the number of chips
	public:
		Environment(vector<Player> &players_, int numberOfChips_) : numberOfChipsPerPlayer(numberOfChips_), players(players_), numberOfChipsInCenter(0) {}

		void setCenterChips(int numberOfChipsInCenter_) {
			numberOfChipsInCenter = numberOfChipsInCenter_;
		}

		//this function is used to add the chips to the center
		void addOneToCenterChips() {
			numberOfChipsInCenter += 1;
		}

		//this function is not used as of now, for the center never gives a chip back.
		void subtractOneToCenterChips() {
			numberOfChipsInCenter -= 1;
		}

		//this function returns the number of chips in the center at any given point.
		int getCenterChips() const {
			return numberOfChipsInCenter;
		}
};
//-----------------------------end of Environment class------------------------------------------
//-----------------------------------------------------------------------------------------------


//////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------show score function-----------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////
void showScore(vector<Player> players) { //this funtion is used to show the current score by taking the players vector and readin the chips count for each player
	int i = 0; //iterator
	for (auto chips : players) {
		cout << players[i].getName() << " has " << players[i].getChips() << " chips" << "\t";
		i++;
	}
}
//---------------------------end of show score function-------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------endOfgame function------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////
bool endOfgame(vector<Player> players_) { //this function will figure out if the game is over by counting who has chips left, if only one then it returns true to end of game.
	int total = 0; //this is used to count the total players that have one or more chips left.
	for (auto player : players_) 
	{
		if (player.getChips() != 0) { //if a player has chips it returns true
			total++; //adds one to the total and if more than one see below
		}		
	}

	if (total > 1) { //if more than one player has chips then the end of game returns false thus the game continues
		return false;
	}
	else
	{
		return true; //otherise the game ends
	}
}
//------------------------end endOfgame funtion---------------------------------------------

/////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------winner function--------------------------------------------------
void winner(vector<Player> players) { //this function finds out who the winner is and prints his name on the screen.

	for (auto player : players) { 
		if (player.getChips() > 0) //if any of the players has more than 0 chips, then he must the winner. of course considering the end of game is called first
		{
			cout << "The winner is: " << player.getName() << endl;
		}
	}
}
//-------------------------end of winner function-------------------------------------------

int main()
{
	int numberOfChipsPerPlayer = 0;
	int numberOfPlayers = 0;
	vector<Player> players;
	vector<string> playersNames;
	int selection = 0;
	string line;

	//the following asks the user if they want to view the rules which is read from a text file
	cout << "Welcome to LCR game! If you wish to read the rules please enter 1, otherwise enter zero: ";
	cin >> selection;
	cout << endl;

	if(selection == 1)
	{ 
		ifstream gameRules("LCRRules.txt");

		if (gameRules.is_open()) {
			while (getline(gameRules, line))
			{
				cout << line << endl;
			}
			gameRules.close();
		}
	}
	
	cout << endl << "Hello, how many are playing: "; //asking the user how many players are going to play
	cin >> numberOfPlayers;

	//storing the players' names
	for(int i = 0; i < numberOfPlayers; i++)
	{
		int count = i+1;
		string name;
		cout << "Enter player number " << count << " name: ";
		cin >> name;
		playersNames.push_back(name);
	}

	//asking the user for the number of chips each player will get
	cout << "How many chips per player: ";
	cin >> numberOfChipsPerPlayer;

//-------this is to print the names of the players
//	for (auto name : playersNames) {
//		cout << name << endl;
//	}

	//creating objects for each new player
	for (auto j = 0; j < numberOfPlayers; j++) {

		Player newPlayer(playersNames[j], numberOfChipsPerPlayer, true);

		players.push_back(newPlayer);
	}
	//------end of creating players-----------------

	//-----creating the environment object----------------
	Environment game(players, numberOfChipsPerPlayer);

	//-----tests if end of game-----------------
	//if the end of game funtion returns false then the while loop continues.
	while (!endOfgame(players)) {

		//cout << "enter while " << endl;

		for (unsigned int i = 0; i < players.size(); i++) { //for each player we are going to do the following... which is play a turn

			//cout << "enter for " << endl;
		
			if (players[i].getChips() != 0) 
			{
				//cout << "enter if " << endl;
							
				auto dice = players[i].roleDice();
				cout << endl << players[i].getName() << " rolled the dice and got: " << endl;

				for (auto dice_ : dice) { //this prints the dice the player got which is stored in a vector and reutrned by the function
					cout << dice_ << endl;
				}

				for (auto dice_ : dice) { //here we taking each result of each dice and acting on it based on the rules

					if (dice_ == 'L') { //if the dice is L then do...
						cout << "Chip moved left" << endl;
						players[i].subtractChips();
						if (players[i].getName() == players.front().getName()) //this checks if the current player is at the front of the vector, this is important becuase if the player is and we move the chip to the left, we would go out of bound
						{
							players.front().addChips(); //if the player is out of bound for the vector we turn around and give it to he first player in the vector
						}
						else {
							players[i - 1].addChips(); //and of course if they are not the first player in the vector they can indeed give it to the one before them.
						}
					}
					else if (dice_ == 'C') { 
						cout << "Chip moved center" << endl;
						players[i].subtractChips(); //calls the functions needed to subtract a chip
						game.addOneToCenterChips(); //adds a chip to the center
					}

					else if (dice_ == 'R') {
						cout << "Chip moved right" << endl;
						players[i].subtractChips();

						if (players[i].getName() == players.back().getName()) //same as the L dice, check if within bounds and act accordingly.
						{
							players.back().addChips();
						}
						else {
							players[i + 1].addChips();
						}
						
						
					}
					else if (dice_ == 'N') {
						//do nothing
					}
				}
					
				showScore(players); //this shows the score of each player everytime someone roles the dice
			}
			else //skips the player that has no chips but keeps him in the game just in case they get chips later on
			{
				//do nothing
			}	
		}
		cout << endl << endl;
		system("pause"); //pause is used to the user of the program can refelct on the results above.
	}

	//Announce winner by calling the function winner
	winner(players);

	system("pause"); //just so we can see the winner before the window closes.

    return 0;
}

