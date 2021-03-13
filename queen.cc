#include "queen.h"
#include <iostream>
#include <vector>
#include "piece.h"
#include "coord.h"
#include "bishop.h"
#include "rook.h"
#include "board.h"

Queen::Queen(int x, int y, std::string colour, Board *b):
   Piece(x, y, getQueenVal(), colour, colour == getWhite() ? 'Q' : 'q', b){}

//compile the list of moves
void Queen::listOfMoves(){
	int x = getX();
	int y = getY();

	if (this->moves){
		delete this->moves;
	}
	std::vector<Coord> *moves = new std::vector<Coord>;


	//treat it as a rook and a bishop at the same time
	Rook r(x, y, getColour(), getBoard());
	Bishop b(x, y, getColour(), getBoard());

	r.listOfMoves();
        std::vector<Coord> *rMoves = r.getMoves();

	b.listOfMoves();
        std::vector<Coord> *bMoves = b.getMoves();


	//add those moves
	for (int i = 0; i < rMoves->size(); i++){
		moves->push_back(rMoves->at(i));
	}
	for (int i = 0; i < bMoves->size(); i++){
		moves->push_back(bMoves->at(i));
	}

	this->moves = moves;
	return;
}
