#include <thread>
#include <iostream>
#include <cctype>

#include "MainMenu.h"
#include "Game.h"
#include "CardNotFoundException.h"
#include "Deck.h"

using namespace std;

MainMenu::MainMenu()
{
	//Begin loading cards upon program startup
	loadingThread = new thread(&JSONCardParser::loadCards, &cardParser);
}

void MainMenu::initiateMenus()
{
	bool quit = false;

	while (!quit)
	{
		system("cls"); //clear screen
		cout << "Welcome to Console Magic: The Gathering!" << endl << endl;

		//Present user with options
		//New game
		//Build deck
		//Card viewer

		cout << "1. [P]lay a game of Magic." << endl;
		cout << "2. [D]eck Builder" << endl;
		cout << "3. [C]ard Viewer" << endl;
		cout << "4. [A]bout" << endl;
		cout << "5. [Q]uit" << endl << endl;

		cin >> userInput;
		system("cls");

		userInput = toLowerCase(userInput);

		if (userInput == "1" || userInput == "p" || userInput == "play")
			runGame();
		else if (userInput == "2" || userInput == "d" || userInput == "deck builder")
			runDeckBuilder();
		else if (userInput == "3" || userInput == "c" || userInput == "card viewer")
			runCardViewer();
		else if (userInput == "4" || userInput == "a" || userInput == "about")
			aboutScreen();
		else if (userInput == "5" || userInput == "q" || userInput == "quit")
			quit = true;
		else
			cerr << "Invalid input. Please try again." << endl;

		if (!quit)
		{
			cout << "Press any key followed by enter to return to the main menu" << endl;
			cin >> userInput;
		}
	}
}

void MainMenu::runGame()
{
	//start making a Game object and then run the game from there
	//Game game;
}

void MainMenu::runDeckBuilder()
{
	//wait for the loading thread to finish
	if (!cardParser.doneLoading())
	{
		cout << "\n\nLoading cards...";
		loadingThread->join();
		cout << "\nDone!";
	}

	//present options
	//create deck
	//quit
	cout << "\n\nDeck Builder: ";
	bool quit = false;
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	while (!quit)
	{
		cout << "\n1. [B]uild new deck"
			<< "\n2. [E]dit existing deck"
			<< "\n3. [Q]uit";

		string option;
		cin >> option;
		option = toLowerCase(option);
		system("cls");

		if (option == "q" || option == "quit" || option == "3")
			quit = true;
		else if (option == "2" || option == "e" || option == "edit")
		{
			cout << "NOT YET IMPLEMENTED";
		}
		else if (option == "1" || option == "b" || option == "build")
		{
			Deck deck;


			//TODO: implement deck builder

			/*try
			{
				Card myCard = cardParser.getCard(myCardName);
				myCard.printCard();
			}
			catch (CardNotFoundException c)
			{
				cerr << c.what();
			}*/
		}
	}

}

void MainMenu::runCardViewer()
{
	if (!cardParser.doneLoading())
	{
		cout << "Loading cards...";
		loadingThread->join();
		cout << "\nDone!";
	}
	
	cout << "\n\nCard viewer: ";
	bool quit = false;
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	while (!quit)
	{
		cout << "\nPlease type in a card name to view its' attributes,\n";
		cout << "or type QUIT to quit.\n";

		string myCardName;
		getline(cin, myCardName);
		

		system("cls");

		if (myCardName == "QUIT")
		{
			quit = true;
		}
		else
		{
			try
			{
				Card myCard = cardParser.getCard(myCardName);
				myCard.printCard();
			}
			catch (CardNotFoundException c)
			{
				cerr << c.what();

				//find similar cards
				nlohmann::json similarCards = cardParser.cardsSimilarTo(myCardName);
				
				if (!similarCards.empty())
				{
					cout << "\nSimilar cards:";
					for (size_t i = 0 ; i < similarCards.size() ; i++)
					{
						cout << i + 1 << ". " << similarCards.at(i).at("name");
					}

					bool validInput = false;

					while (!validInput)
					{
						//select a card
						size_t selectCard;
						cin >> selectCard;

						if (selectCard > 0 && selectCard <= similarCards.size())
						{
							Card myCard(similarCards[selectCard]);
						}
						else
						{
							cerr << "Invalid input. Try again.\n";
						}
					}
				}
			}
		}
	}
}

void MainMenu::aboutScreen()
{
	cout << "CONSOLE MAGIC: THE GATHERING";
	cout << "\n\nBy Devin Gunay";


	cout << "\n\nIn order to update your card database, please visit http://mtgjson.com";
	cout << "\nand download the latest AllCards.json file.";
	cout << "\n\nMagic: The Gathering is property of Wizards of the Coast LLC.";
	cout << "\nAllCards.json contains data that is Copyright � Wizards of the Coast - All Rights Reserved";
	cout << "\n\nThis program is not affiliated with Wizards of the Coast in any way.";

	cout << endl;
}

//returns the passed string, with all lowercase letters
string MainMenu::toLowerCase(const string & str)
{
	string newString = str;

	if (str.length() > 1)
	{
		auto iter = newString.begin();

		while (iter != newString.end())
		{
			if (isupper(*iter))
				*iter = tolower(*iter);

			iter++;
		}
	}
	
	
	return newString;
}

MainMenu::~MainMenu()
{
	if (loadingThread->joinable())
	{
		cout << "Must finish loading AllCards.json before exiting...";
		loadingThread->join();
	}
		
	delete loadingThread;
}
