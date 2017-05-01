#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>
#include <Windows.h>

//List of all game functions.
int mainmenu();
void boards(int i);
void instructions();
char* difficulty(char* currentWord);
bool Replay(bool replay);
int drawTheGame(int length, bool guessedCorrectly[], char* currentWord, char wrongLetters[], int counter);
char guessALetter();
int letterCheck(int oldFalseCounter, int newFalseCounter, int len, char* currentWord, char guessedLetter, bool guessedCorrectly[]);
int failStateCounterUpdate(int oldFalseCounter, int newFalseCounter, char wrongLetters[], int failStateCounter, char guessedLetter);
void win(int exit);
int checkWonOrLost(int failStateCounter,int exit, int counter, int len);

//Main game loop
int main()
{
	//Replay loop: will break if user does not want to replay
	bool replay = true;
	while (replay == true)
	{
		//Go to main menu and get the user choice
		int c = mainmenu();
		//variable that counts if player has lost
		int failStateCounter = 0;
		//switch that changes based on what the player chose.
		switch (c)
		{
			//play game
		case 1:
		{
			//Variables
			bool guessedCorrectly[11] = { false };//if player guessed letter n correctly flase n will be set to true
			int counter = 0;//if counter = length of word then the game has been won because all letters have been guessed correctly
			char wrongLetters[6] = { ' ',' ',' ',' ',' ',' ' };//keeps track of all the wrongly guessed letters.
			char guessedLetters[17] = { ' ' };//array that holds all of the letters that have been guessed.
			char guessedLetter = ' ';//variable that will hold the value of the players guessed letter
			int oldFalseCounter = 0;//counter that will keep track of the past number of flase's in the guessedCorrectly array
			char* currentWord = " ";//variable that will hold the current secret word
			int exit = 0;//varible that dictates when to end the game loop and if the player has won or not
			int noOfGuessedLetters = 0;//variable that keeps track of the number of latters that have been guessed

			system("CLS");
			//Get the random word by selecting the difficulty and then the word
			currentWord = difficulty(currentWord);//function that will ask the player what difficulty they want to play and select the random word.
			size_t len = std::strlen(currentWord);//length of current word
			int newFalseCounter = len;//variable that will keep track of the current number of false's in the guessedCorrectly array

									  //main game loop
			while (exit == 0)
			{
				system("cls");
				//function that will draw the boards based on what failstate the player is at.
				boards(failStateCounter);

				//function that will check the letter to see if it matches any of the letters in the word, counts them up and outputs them.
				oldFalseCounter = letterCheck(oldFalseCounter, newFalseCounter, len, currentWord, guessedLetter, guessedCorrectly);
				//function that will see if any of the elements in the guessedCorrectly array have changed, if not then the failStateCounter will be the same as the oldFailStateCoutner 
				failStateCounter = failStateCounterUpdate(oldFalseCounter, newFalseCounter, wrongLetters, failStateCounter, guessedLetter);
				//function that will draw the rest of the game elements and output how many letters have been guessed correctly.
				counter = drawTheGame(len, guessedCorrectly, currentWord, wrongLetters, counter);
				//function that checks if the game has been won or lost.
				exit = checkWonOrLost(failStateCounter, exit, counter, len);

				//if the game is won or lost then another letter input is not needed.
				if (exit == 0)
				{
					//function to let the player guess a letter
					guessedLetter = guessALetter();

					//START OF VALIDATION//
					bool alreadyEntered = true;//if the letter has not already been enterd this will be set to false
					bool passedTest = true;//if it is set to true this will break the loop and continue with the rest of the program
										   //loop will not exit unless already entered is false.
					while (alreadyEntered == true)
					{
						//check if the input was a letter.
						while (96 >= int(guessedLetter) || 123 <= int(guessedLetter))
						{
							//tells the user the input is invalid and prompts for another input.
							std::cout << std::endl << guessedLetter << " is invalid.\nGuess a letter: ";
							std::cin >> guessedLetter;
							guessedLetter = tolower(guessedLetter);
						}
						//check if letter enterd has already been entered
						for (int i = 0; i < 15; i++)
						{
							passedTest = true;//sets to true after every loop
											  //checks to see if letter entered has already been entered
							if (guessedLetter == guessedLetters[i])
							{
								//prompts user for a new letter.
								std::cout << "\nAlready Entered, please guess again: ";
								std::cin >> guessedLetter;
								guessedLetter = tolower(guessedLetter);
								passedTest = false;
								//breaks the for loop
								break;
							}
						}
						//if the test was passed and no letter was found to match.
						if (passedTest == true)
						{
							//sets the number element in the numberOfGuessedLetters to equal the guessed letter so it cant be guessed again.
							guessedLetters[noOfGuessedLetters] = guessedLetter;
							noOfGuessedLetters++;
							//breaks the while loop
							alreadyEntered = false;
						}
					}
					//END OF VALIDATION//
				}
			}
			//END OF MAIN GAME LOOP//

			//function that will check if the game has been won or lost and output as appropriate.
			win(exit);
			//prompts the user if they want to play again.
			replay = Replay(replay);
			break;
		}
		//instructions
		case 2:
		{
			instructions();
			break;
		}
		//exit game
		case 3:
		{
			std::cout << "Goodbye... \n";
			Sleep(500);
			replay = false;
			break;
		}
		//if the user has entered an invalid option.
		default:
		{
			system("cls");
			std::cout << "Invalid Response.\n";
			break;
		}
		}
	}
}

//Main Menu of the game
int mainmenu()
{
	//print out menu
	std::cout << "#######################\n#       HANGMAN       #\n#######################\n_______________________\n";
	std::cout << "Play:" << std::setw(19) << "Press 1\n";
	std::cout << "Instructions:" << std::setw(11) << "press 2\n";
	std::cout << "Exit:" << std::setw(19) << "press 3\n";
	
	//switch that takes the users choice of what they want to do
	int c = 0;
	std::cin >> c;
	return c;
}

//user selects the difficulty of the game
char* difficulty(char* currentWord)
{
	//sets the current word to be empty
	currentWord = " ";
	bool esc = false;//varible that will make the loop repeat if given an invalid response
	while (esc == false)
	{
		//Print out menu
		std::cout << "#######################\n#       HANGMAN       #\n#######################\n_______________________\n";
		std::cout << "Slect a difficly\n";
		std::cout << "Easy:" << std::setw(19) << "Press 1\n";
		std::cout << "Medium:" << std::setw(17) << "press 2\n";
		std::cout << "Hard:" << std::setw(19) << "press 3\n";
		//get the user input for what difficulty they want
		int x = 0;
		std::cin >> x;
		//all difficulty word arrays.
		char* easyWords[5] = { "cat", "hat", "sat", "dog", "egg" };
		char* medWords[5] = { "logical", "pointer", "hangman", "keyboard", "orange" };
		char* hardWords[5] = { "kickboxer", "equalizing", "meaningless", "jaywalking", "microwave" };
		//seading the rand
		srand(time(NULL));
		//switch that chooses the difficulty
		switch (x)
		{
		//Easy words
		case 1:
		{
			currentWord = easyWords[rand() % 5];
			esc = true;
			break;
		}
		//Medium words
		case 2:
		{
			currentWord = medWords[rand() % 5];
			esc = true;
			break;
		}
		//Hard words
		case 3:
		{
			currentWord = hardWords[rand() % 5];
			esc = true;
			break;
		}
		//if the user has entered an invalid option
		default:
		{
			system("cls");
			std::cout << "Invalid Response.\n";
			break;
		}
		}
	}
	return currentWord;
}

//game outputs all the information the user needs and counts up all the correctly guessed letters
int drawTheGame(int length, bool guessedCorrectly[], char* currentWord, char wrongLetters[], int counter)
{
	//resets the counter.
	counter = 0;
	for (int i = 0; i < length; i++)
	{
		//if letter hasnt been guessed outputs a dash else outputs the correctly guessed letter.
		if (guessedCorrectly[i] == true)
		{
			std::cout << currentWord[i];
			counter++;
		}
		else
		{
			std::cout << "_ ";
		}
	}
	//outputs all the incorrect guesses
	std::cout << "\nIncorrect Guesses: ";
	for (int i = 1; i < 6; i++)
	{
		//if there is nothing in the wron letters array element print out nothing else print a dash
		if (wrongLetters[i] != ' ')
		{
			std::cout << wrongLetters[i] << " ";
		}
		else
		{
			std::cout << "_ ";
		}
	}
	return counter;
}

//allows user to input a letter.
char guessALetter()
{
	//get the user input
	std::cout << "\nGuess a letter: ";
	char guessedLetter = ' ';
	std::cin >> guessedLetter;
	guessedLetter = tolower(guessedLetter);
	return guessedLetter;
}

//checks if the letter was guessed correcly
int letterCheck(int oldFalseCounter ,int newFalseCounter, int len, char* currentWord, char guessedLetter, bool guessedCorrectly[])
{
	//checking if any letters have been guessed correctly
	oldFalseCounter = newFalseCounter;
	newFalseCounter = 0;
	for (int i = 0; i < len; i++)
	{
		//check if the letter in i matches the guessed letter
		if (currentWord[i] == guessedLetter)
		{
			guessedCorrectly[i] = true;
		}
		//count up number of letters that have remained in the same state (correct or incorrect)
		if (guessedCorrectly[i] == false)
		{
			newFalseCounter++;
		}
	}
	return oldFalseCounter, newFalseCounter;
}

//adds the letter to wrong letters if it was guessed wrong and updates the failstatecounter.
int failStateCounterUpdate(int oldFalseCounter, int newFalseCounter, char wrongLetters[], int failStateCounter, char guessedLetter)
{
	//if the newfalsecounter is the same as the oldfalsecounter then no letter was guessed correctly that time so loose a life.
	if (oldFalseCounter == newFalseCounter)
	{
		wrongLetters[failStateCounter] = guessedLetter;
		failStateCounter++;
	}
	return failStateCounter;
}

//checks if the game has been won or lost
int checkWonOrLost(int failStateCounter, int exit, int counter, int len)
{
	//if the plater is out of lives
	if (failStateCounter == 6)
	{
		exit = 1;
	}
	//if the number of correct letters = the length of the word
	if (counter == len)
	{	
		exit = 2;
	}
	return exit;
}

//will output win or loose as apropriate
void win(int exit)
{
	system("cls");
	//if the game has been won
	if (exit == 2)
	{
		std::cout << "\n\nYOU WIN!!!\n\n";
	}
	//if the game has been lost
	else
	{
		std::cout << "\n\n       _______\n";
		std::cout << "      |/      |\n";
		std::cout << "      |      (_)\n";
		std::cout << "      |      \\|/\n";
		std::cout << "      |       |\n";
		std::cout << "      |      / \\ \n";
		std::cout << "      |\n";
		std::cout << "     _|___\n";
		std::cout << "\n\nYOU LOOSE!!!\n\n";
	}
}

//function that draws the boards
void boards(int i)
{
	switch (i)
	{
	case 0:
		std::cout << "\n\n       \n";
		std::cout << "      |     \n";
		std::cout << "      |     \n";
		std::cout << "      |      \n";
		std::cout << "      |     \n";
		std::cout << "      |     \n";
		std::cout << "      |     \n";
		std::cout << "     _|___\n";
		break;
	case 1:
		std::cout << "\n\n       _______\n";
		std::cout << "      |/      \n";
		std::cout << "      |      \n";
		std::cout << "      |      \n";
		std::cout << "      |      \n";
		std::cout << "      |      \n";
		std::cout << "      |      \n";
		std::cout << "     _|___\n";
		break;
	case 2:
		std::cout << "\n\n       _______\n";
		std::cout << "      |/      |\n";
		std::cout << "      |      (_)\n";
		std::cout << "      |      \n";
		std::cout << "      |      \n";
		std::cout << "      |      \n";
		std::cout << "      |      \n";
		std::cout << "     _|___\n";
		break;
	case 3:
		std::cout << "\n\n       _______\n";
		std::cout << "      |/      |\n";
		std::cout << "      |      (_)\n";
		std::cout << "      |       |\n";
		std::cout << "      |       |\n";
		std::cout << "      |      \n";
		std::cout << "      |\n";
		std::cout << "     _|___\n";
		break;
	case 4:
		std::cout << "\n\n       _______\n";
		std::cout << "      |/      |\n";
		std::cout << "      |      (_)\n";
		std::cout << "      |      \\|/ \n";
		std::cout << "      |       |\n";
		std::cout << "      |      \n";
		std::cout << "      |      \n";
		std::cout << "     _|___\n";
		break;
	}
}

//Does the player want to replay the game
bool Replay(bool replay)
{
	//variable that means the input has to be valid before continuing
	bool esc = false;
	while (esc == false)
	{
		//propmpts the user if they want to replay
		std::cout << "Play again?(y/n): ";
		char playAgain = ' ';
		std::cin >> playAgain;
		switch (playAgain)
		{
		case 'y':
		{
			//if yes the game will restat
			std::cout << "Restarting... ";
			Sleep(500);
			system("cls");
			replay = true;
			esc = true;
			break;
		}
		case 'n':
		{
			//if no the game will stop
			std::cout << "Goodbye... \n";
			Sleep(500);
			replay = false;
			esc = true;
			break;
		}
		default:
		{
			//if not a valid response
			std::cout << "\nInvalid Response\n";
			break;
		}
		}
	}
	return replay;
}

//instuctions for the game
void instructions()
{
	system("cls");
	std::cout << "#######################\n#       HANGMAN       #\n#######################\n_______________________\n";
	std::cout << "The objective is to guess \nthe word that is hidden in \nthe blanks at the top, \nwhen you guess incorrectly \nthe hangman will be drawn \na bit more. \nYou must guess the word before \nthe man is hung.\n\nPress any key to go back to the menu.\n\n";
	system("pause");
	system("cls");
	main();
}