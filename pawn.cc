#include "pawn.h"
#include <iostream>
#include "piece.h"
#include <vector>

Pawn::Pawn(int x, int y, std::string colour, Board *b):
   firstMoveMade(false), jumpedTwo(false),
   Piece(x, y, getPawnVal(), colour, colour == getWhite() ? 'P' : 'p', b){}

Pawn::~Pawn(){}

bool Pawn::getFirstMoveMade(){
	return firstMoveMade;
}


//compile a list of moves
void Pawn::listOfMoves(){
	int x = getX();
	int y = getY();

	if (this->moves){
		delete this->moves;
	}

	std::vector<Coord> *moves = new std::vector<Coord>;


	if (getColour() == getBlack()){  //black piece (ie. moving down)

		Piece *forward = board->getPiece(x, y - 1);
                Piece *leftDiag = x - 1 >= 0 ? board->getPiece(x - 1, y - 1) : 0;
                Piece *rightDiag = x + 1 <= 7 ? board->getPiece(x + 1, y - 1): 0;
		Piece *doubleForward = board->getPiece(x, y - 2);
		Piece *enPassant = board->getEnPassant() && board->getEnPassantColour() != getColour() ? board->getPiece(board->getEnPassant()->getX(), board->getEnPassant()->getY() + 1) : 0;
		

		//add all the possible moves
		if (!forward){
			moves->push_back(*(new Coord(x, y - 1)));
		}
		if (!doubleForward && !firstMoveMade && !forward){
			moves->push_back(*(new Coord(x, y - 2)));
		}
		if (x - 1 >= 0 && leftDiag && leftDiag->getColour() == getWhite()){
			moves->push_back(*(new Coord(x - 1, y - 1)));
		}
		if (x + 1 <= 7 && rightDiag && rightDiag->getColour() == getWhite()){
			moves->push_back(*(new Coord(x + 1, y - 1)));
		}
		if (enPassant && (enPassant->getX() == getX() + 1 || enPassant->getX() == getX() - 1) && enPassant->getY() == getY()){
			moves->push_back(*(new Coord(enPassant->getX(), y - 1)));
		}
	} else { //white piece (ie. moving up)

                Piece *forward = board->getPiece(x, y + 1);
		Piece *leftDiag = x - 1 >= 0 ? board->getPiece(x - 1, y + 1) : 0;
		Piece *rightDiag = x + 1 <= 7 ? board->getPiece(x + 1, y + 1): 0;
		Piece *doubleForward = board->getPiece(x, y + 2);
		Piece *enPassant = board->getEnPassant() && board->getEnPassantColour() != getColour() ? board->getPiece(board->getEnPassant()->getX(), board->getEnPassant()->getY() - 1) : 0;


		//add all possible moves
                if (!forward){
                        moves->push_back(*(new Coord(x, y + 1)));
                }
                if (!doubleForward && !firstMoveMade && !forward){
                        moves->push_back(*(new Coord(x, y + 2)));
                }
                if (x - 1 >= 0 && leftDiag && leftDiag->getColour() != getWhite()){
                        moves->push_back(*(new Coord(x - 1, y + 1)));
                }
                if (x + 1 <= 7 && rightDiag && rightDiag->getColour() != getWhite()){
                        moves->push_back(*(new Coord(x + 1, y + 1)));
                }
                if (enPassant && (enPassant->getX() == getX() + 1 || enPassant->getX() == getX() - 1) && enPassant->getY() == getY()){
                        moves->push_back(*(new Coord(enPassant->getX(), y + 1)));
                }

	}
	this->moves = moves;
}

void Pawn::moveStats()
{
	firstMoveMade = true;
	return;
}
