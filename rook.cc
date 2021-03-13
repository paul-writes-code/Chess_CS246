#include "rook.h"
#include <vector>
#include <iostream>
#include "board.h"

Rook::Rook(int x, int y, std::string colour, Board *b):
   Piece(x, y, getRookVal(), colour, colour == getWhite() ? 'R' : 'r', b), firstMoveMade(false){}

Rook::~Rook(){}


//compile a list of moves
void Rook::listOfMoves(){
	int x = getX();
	int y = getY();

	if (this->moves){
		delete this->moves;
	}

	std::vector<Coord> *moves = new std::vector<Coord>;

	
	//check to the right
	for (int i = x + 1; i <= 7; i++){
		Piece *p = board->getPiece(i, y);
		if (!p){
			moves->push_back(*(new Coord(i, y)));
		} else if (p->getColour() != getColour()){
			moves->push_back(*(new Coord(i, y)));
			break;
		} else {
			break;
		}
	}


	//check to the left
	for (int i = x - 1; i >= 0; i--){
		Piece *p = board->getPiece(i, y);
		if (!p){
			moves->push_back(*(new Coord(i, y)));
		} else if (p->getColour() != getColour()){
			moves->push_back(*(new Coord(i, y)));
			break;
		} else {
			break;
		}
	}


	//check above
        for (int i = y + 1; i <= 7; i++){
                Piece *p = board->getPiece(x, i);
                if (!p){
                        moves->push_back(*(new Coord(x, i)));
                } else if (p->getColour() != getColour()){
                        moves->push_back(*(new Coord(x, i)));
                        break;
                } else {
			break;
		}
        }


	//check below
        for (int i = y - 1; i >= 0; i--){
                Piece *p = board->getPiece(x, i);
                if (!p){
                        moves->push_back(*(new Coord(x, i)));
                } else if (p->getColour() != getColour()){
                        moves->push_back(*(new Coord(x, i)));
                        break;
                } else {
			break;
		}
        }
	this->moves = moves;
}

void Rook::moveStats()
{
	firstMoveMade = true;
	return;
}

bool Rook::canCastle()
{
	return !firstMoveMade;
}
