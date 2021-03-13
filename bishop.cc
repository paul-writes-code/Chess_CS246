#include "bishop.h"
#include "piece.h"
#include <vector>
#include <iostream>
#include "coord.h"
#include "board.h"

Bishop::Bishop(int x, int y, std::string colour, Board *b):
   Piece(x, y, getBishopVal(), colour, colour == getWhite() ? 'B' : 'b', b){}

Bishop::~Bishop(){}


//compiles the list of moves
void Bishop::listOfMoves(){
	int x = getX();
	int y = getY();

	if (this->moves){
		delete this->moves;
	}

	std::vector<Coord> *moves = new std::vector<Coord>;
	
	x ++;
	y ++;
	
	//check upper right diagonal
	while (x <= 7 && y <= 7){
		Piece *p = board->getPiece(x, y);
		if (!p){
			moves->push_back(*(new Coord(x, y)));
		} else{
			if (p && p->getColour() != getColour()){
				moves->push_back(*(new Coord(x, y)));
			}
			break;
		}
		x++;
		y++;
	}

	x = getX();
	y = getY();

	x ++;
	y --;
	

	//check lower right diagonal
	while (x <= 7 && y >= 0){
		Piece *p = board->getPiece(x, y);
		if (!p){
			moves->push_back(*(new Coord(x, y)));
		} else{
			if (p && p->getColour() != getColour()){
				moves->push_back(*(new Coord(x, y)));
			}
			break;
		}
		x++;
		y--;
	}

	x = getX();
	y = getY();

	x --;
	y ++;

	//check upper left diagonal
        while (x >= 0 && y <= 7){
                Piece *p = board->getPiece(x, y);
                if (!p){
                        moves->push_back(*(new Coord(x, y)));
                } else {
			if (p && p->getColour() != getColour()){
                        	moves->push_back(*(new Coord(x, y)));
                        }
			break;
                }
                x--;
                y++;
        }

	x = getX();
	y = getY();

	x --;
	y --;


	//check lower left diagonal
        while (x >= 0 && y >= 0){
                Piece *p = board->getPiece(x, y);
                if (!p){
                        moves->push_back(*(new Coord(x, y)));
                } else {
			if (p && p->getColour() != getColour()){
                        	moves->push_back(*(new Coord(x, y)));
                        }
			break;
                }
                x--;
                y--;
        }
	this->moves = moves;
}
