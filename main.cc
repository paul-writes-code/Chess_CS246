#include "board.h"
#include "piece.h"
#include "coord.h"
#include "ai.h"
#include "bishop.h"
#include "rook.h"
#include "queen.h"
#include "pawn.h"
#include "king.h"
#include "knight.h"
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;

//TODO:
	// bullet proof all commands

// commands:
//  game white-player black-player (human, computer[1-4])
//  resign
//  move start-coord end-coord (move e2 e4)
//  setup
//	+ K e1 places K on square e1, overwriting current piece
//	- e1 removes piece from square e1 (no action if nothing there)
//	= colour makes it colour's turn
//	done exists setup mode


//Variables and constants used to keep track of values throughout the main loop

string turn;
const string invalid = "Invalid input - try again";
const string invalidSetup = "Unable to exit setup mode. Ensure there is exactly one white king, one black king, and no pawns in the first and last rows and try again.";
double blackScore = 0;
double whiteScore = 0;
bool gameInProgress = false;
bool beenSetup = false;

//a function which consumes a string and returns the same string in all caps

string toCaps(string str)
{
	string s = str;
	for (int i = 0; i < s.size(); i ++)
	{
		if (s[i] >= 'a' && s[i] <= 'z')
		{
			s[i] -= 'a' - 'A';
		}
	}
	return s;
}


//this function checks all victory conditions and acts apropriatly

void gameOver(Board *b){
	if (!(b->isWon()))
	{
		string checkmate = "";
		string check = "";
	
  		checkmate = b->inCheckmate(); // reads in the colour that is in checkmate "" by default
	
       		if (checkmate == "") check = b->inCheck(); // read in the colour of check

		if (checkmate == Piece::getWhite()){
           		cout << "Checkmate! Black wins!" << endl;
                	blackScore++;
                	gameInProgress = false;
			beenSetup = false;
       		} else if (checkmate == Piece::getBlack()){
                	cout << "Checkmate! White wins!" << endl;
                	whiteScore++;
               		gameInProgress = false;
			beenSetup = false;
       		} else if (check == Piece::getWhite()){
           		cout << "White is in check." << endl;
       		} else if (check == Piece::getBlack()){
           	cout << "Black is in check." << endl;
		} else if (b->isStalemate()){
			cout << "Stalemate!" << endl;
			whiteScore += 0.5;
			blackScore += 0.5;
			gameInProgress = false;
			beenSetup = false;
		}
	}
	else
	{
		if (turn == Piece::getBlack()) // white wins
		{
			cout << "White wins!" << endl;
			whiteScore ++;
			gameInProgress = false;
			beenSetup = false;
		}
		else
		{
			cout << "Black wins!" << endl;
			blackScore ++;
			gameInProgress = false;
			beenSetup = false;
		}
	}
}


// a function which enters setup mode

bool setupMode(Board *b){
	b->clearBoard(); // clears the board
	string c;
   	bool setupDone = false;
	string commandLine = "";
	istringstream *pss = NULL; // a stringstream to use to read line by line


       	while (!setupDone){  // loops through until done is read
		cout << "Enter a command: ";
		getline(cin, commandLine); // readin a line
		delete pss;
		pss = new istringstream(commandLine);
		*pss >> c; // read from the string stream
 
		if (cin.eof()){ // EOF read
			return false;
		} else if (c == "+"){ // start add portion
      			char piece, coordx;
                   	int coordy;
                   	string colour;
                   	int x, y;

			if (*pss >> piece && *pss >> coordx && *pss >> coordy){ // read in the piece and coordinates

	                   	x = coordx - 'a';
	                   	y = coordy - 1;
	
				if (x > 7 || x < 0 || y > 7 || y < 0){
					cout << invalid << endl;
				} else {
			
					// get the colour

		                   	if (piece >= 97 && piece <= 122){
		                          	colour = Piece::getBlack();
		                   	} else if (piece >= 65 && piece <= 90){
		                          	colour = Piece::getWhite();
		                    	}
	
					// make the piece

		                   	if (piece == 'K' || piece == 'k'){
		                         	b->add(*(new King(x, y, colour, b)));
		                   	} else if (piece == 'Q' || piece == 'q'){
		                          	b->add(*(new Queen(x, y, colour, b)));
		                   	} else if (piece == 'B' || piece == 'b'){
		                           	b->add(*(new Bishop(x, y, colour, b)));
		                    	} else if (piece == 'R' || piece == 'r'){
			                         b->add(*(new Rook(x, y, colour, b)));
		                    	} else if (piece == 'P' || piece == 'p'){
		                          	b->add(*(new Pawn(x, y, colour, b)));
		                  	} else if (piece == 'N' || piece == 'n'){
		                          	b->add(*(new Knight(x, y, colour, b)));
		                    	} else {
		                          	cout << invalid << endl;
					}
				}
			} else {
				cout << invalid << endl;
			}
            	} else if (c == "-"){ // remove a piece
                    	char coordx;
                        int coordy;
                        int x, y;

			if (*pss >> coordx && *pss >> coordy){ // get coordinates

	                        x = coordx - 'a';
	                        y = coordy - 1;

				if (x >= 0 && x < 8 && y >= 0 && y < 8){
					Coord coord(x, y);
	       	                	b->remove(coord);
				} else {
					cout << invalid << endl;
				}
			} else {
				cout << invalid << endl;
			}
             	} else if (c == "="){ // set turn to colour (ie. colour goes first, White default)
	              	string colour;
			
			if (*pss >> colour){ // read in the colour
	                      	if (toCaps(colour) == Piece::getWhite()){
	                        	turn = Piece::getWhite();
	                       	} else if (toCaps(colour) == Piece::getBlack()){
	                           	turn = Piece::getBlack();
	                       	} else {
	                           	cout << invalid << endl;
	                       	}
			} else {
				cout << invalid << endl;
			}
              	} else if (c == "done"){ // exit setup mode
                       	if (!b->validBoard()){
                           	cout << invalidSetup << endl;
                       	} else {
				b->setup();
                        	return true;
                     	}
        	} else {
			cout << invalid << endl;
		}
		if (!setupDone){ // print the board
			cout << *b;
		}
	}
}


// main function

int main(){
	string white, black;
	turn = Piece::getWhite();
	Board *b = Board::getInstance();
	string commandLine = "";
	string command = "";
	bool done = false;
	bool setup = false;
	bool whitePlayer = false;
	bool blackPlayer = false;
	int whiteAILevel = -1;
	int blackAILevel = -1;
	bool whiteInput = false;
	bool blackInput = false;
	bool justErrored = false;

	istringstream * pss = NULL;

	cout << *b; // print the board at the start

	while(!done){ // loop while not EOF
		justErrored = true; // default error returned
		if (gameInProgress) // print who's turn it is
		{
			cout << "It is " << b->getString(turn) << "'s move" << endl;
		}
		cout << "Enter a command: ";
		getline(cin, commandLine); // get the command line
		delete pss;
		pss = new istringstream(commandLine);
		*pss >> command;
		if (cin.eof()) // end if EOF
		{
			done = true;
		}
		else if (command == "game" && !gameInProgress && *pss >> white && *pss >> black){ // start a new game
			if (!beenSetup) turn = Piece::getWhite();
			whiteInput = true;
			blackInput = true;

			// choose and apropriate AI

			if (white == "human"){
				whitePlayer = true;
				whiteAILevel = -1;
			} else if (white == "computer[1]"){
				whitePlayer = false;
				whiteAILevel = 1;
			} else if (white == "computer[2]"){
				whitePlayer = false;
				whiteAILevel = 2;
			} else if (white == "computer[3]"){
				whitePlayer = false;
				whiteAILevel = 3;
			} else if (white == "computer[4]"){
				whitePlayer = false;
				whiteAILevel = 4;
			} else {
				whiteInput = false;
			}

                       	if (black == "human"){
                               	blackPlayer = true;
                                blackAILevel = -1;
                       	} else if (black  == "computer[1]"){
                              	blackPlayer = false;
                           	blackAILevel = 1;
                       	} else if (black  == "computer[2]"){
                         	blackPlayer = false;
                             	blackAILevel = 2;
                      	} else if (black == "computer[3]"){
                            	blackPlayer = false;
                               	blackAILevel = 3;
                       	} else if (black == "computer[4]"){
                              	blackPlayer = false;
                             	blackAILevel = 4;
                      	} else {
                             	blackInput = false;
                       	}

			if (!blackInput || !whiteInput){
				whitePlayer = false;
				blackPlayer = false;
				whiteAILevel = -1;
				blackAILevel = -1;
			} else {
				b->start();
				gameInProgress = true;
				justErrored = false;
			}

			if (gameInProgress) gameOver(b);

		} else if (command == "resign" && gameInProgress){ // exit a game
			justErrored = false;
			gameInProgress = false;
			beenSetup = false;
			if (turn == Piece::getWhite()){
				cout << "Black wins!" << endl;
				blackScore ++;
			} else {
				cout << "White wins!" << endl;
				whiteScore ++;
			}
		} else if (command == "move" && gameInProgress){ // perform a move
			char coord1x, coord2x, promote;
			int coord1y, coord2y;
			int startX, startY, endX, endY;

			if ((turn == Piece::getWhite() && whiteAILevel != -1) || (turn == Piece::getBlack() && blackAILevel != -1)){ // make an AI move
				if (turn == Piece::getWhite())
				{
					aiMove(whiteAILevel, b, turn);
					turn = Piece::getBlack();
					justErrored = false;
				}
				else
				{
					aiMove(blackAILevel, b, turn);
					turn = Piece::getWhite();
					justErrored = false;
				}
			} else {
				if (*pss >> coord1x && *pss >> coord1y && *pss >> coord2x && *pss >> coord2y){ // get a human move
					startX = coord1x - 'a';
					startY = coord1y - 1;
					endX = coord2x - 'a';
					endY = coord2y - 1;

					if (startX < 8 && startX >= 0 & startY < 8 && startY >= 0 && endX < 8 && endX >= 0 && endY < 8 && endY >= 0)
					{
						Coord c(startX, startY);
						Coord d(endX, endY);
						char promotion = 'x';
						if (b->getPiece(startX, startY) && (b->getPiece(startX, startY)->getColour() == turn) && (((endY == 7 || endY == 0) && (b->getPiece(startX, startY)->getSymbol() == 'p' || b->getPiece(startX, startY)->getSymbol() == 'P')) ? (*pss >> promotion) : true) && b->move(c, d))
						{ // perform pawn promotion
							while (promotion != 'x')
							{
								if (promotion == 'q' || promotion == 'Q')
								{
									b->add(*(new Queen(endX, endY, turn, b)));
									break;
								}
								else if (promotion == 'b' || promotion == 'B')
								{
									b->add(*(new Bishop(endX, endY, turn, b)));
									break;
								}
								else if (promotion == 'n' || promotion == 'N')
								{
									b->add(*(new Knight(endX, endY, turn, b)));
									break;
								}
								else if (promotion == 'r' | promotion == 'R')
								{
									b->add(*(new Rook(endX, endY, turn, b)));
									break;
								}
								else
								{
									cout << "That is not a correct replacement" << endl;
									cin >> promotion;
								}
							}
							turn = turn == Piece::getBlack() ? Piece::getWhite() : Piece::getBlack();
							justErrored = false;
						}
					}
				} else {
					cin.clear();
					cin.ignore();
				}
			}
		} else if (command == "setup" && !gameInProgress){ // call the setup function
			if (!setupMode(b)) done = true;
			beenSetup = true;
			cin.clear();
			justErrored = false;
		}
		if (justErrored && !done) // error in command
		{
			cin.clear();
			cout << invalid << endl;
		}
		else
		{
			if (gameInProgress && !done){ // print board
				cout << *b;
				gameOver(b);
			}
		}
	}

	// print the final score
	
	cout << endl;
	cout << "Final Score:" << endl;
	cout << "White: " << whiteScore << endl;
	cout << "Black: " << blackScore << endl;
}
