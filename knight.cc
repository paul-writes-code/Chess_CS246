#include "knight.h"
#include <vector>
#include "board.h"

Knight::Knight(int x, int y, std::string colour, Board *b):
   Piece(x, y, getKnightVal(), colour, colour == getWhite() ? 'N' : 'n', b){}

Knight::~Knight(){}


//compile the list of moves
void Knight::listOfMoves(){
	int x = getX();
	int y = getY();

	if (this->moves){
		delete this->moves;
	}

	std::vector<Coord> *moves = new std::vector<Coord>;

	//check each of the 8 Ls
	if (x + 2 <= 7){
		if (y - 1 >= 0){
			Piece *p = board->getPiece(x + 2, y - 1);
			if ((p && p->getColour() != getColour()) || !p){
				moves->push_back(*(new Coord(x + 2, y - 1)));
			}
		}
		if (y + 1 <= 7){
                        Piece *p = board->getPiece(x + 2, y + 1);
                        if ((p && p->getColour() != getColour()) || !p){
				moves->push_back(*(new Coord(x + 2, y + 1)));
			}
		}
	}
	if (x + 1 <= 7){
		if (y - 2 >= 0){
			Piece *p = board->getPiece(x + 1, y - 2);
			if ((p && p->getColour() != getColour()) || !p){
				moves->push_back(*(new Coord(x + 1, y - 2)));
			}
		}
		if (y + 2 <= 7){
                        Piece *p = board->getPiece(x + 1, y + 2);
      			if ((p && p->getColour() != getColour()) || !p){
				moves->push_back(*(new Coord(x + 1, y + 2)));
			}
		}
	}
	if (x - 1  >= 0){
		if (y - 2 >= 0){
                        Piece *p = board->getPiece(x - 1, y - 2);
                        if ((p && p->getColour() != getColour()) || !p){
				moves->push_back(*(new Coord(x - 1, y - 2)));
			}
		}
		if (y + 2 <= 7){
                        Piece *p = board->getPiece(x - 1 , y + 2);
                        if ((p && p->getColour() != getColour()) || !p){
				moves->push_back(*(new Coord(x - 1, y + 2)));
			}
		}
	}
	if (x - 2 >= 0){
		if (y + 1 <= 7){
                        Piece *p = board->getPiece(x - 2, y + 1);
                        if ((p && p->getColour() != getColour()) || !p){
				moves->push_back(*(new Coord(x - 2, y + 1)));
			}
		}
		if (y - 1 >= 0){
                        Piece *p = board->getPiece(x - 2, y - 1);
                       	if ((p && p->getColour() != getColour()) || !p){
				moves->push_back(*(new Coord(x - 2, y - 1)));
			}
		}
	}

	this->moves = moves;
}
