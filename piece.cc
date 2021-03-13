#include "piece.h"
#include <iostream>
#include "queen.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"
#include "pawn.h"



std::string Piece::white = "WHITE";
std::string Piece::black = "BLACK";
const int Piece::pawnVal = 1;
const int Piece::knightVal = 3;
const int Piece::bishopVal = 3;
const int Piece::rookVal = 5;
const int Piece::queenVal = 9;
const int Piece::kingVal = 100; // or anything greater than the sum of the worth of all pieces


//piece constructor
Piece::Piece(int x, int y, int pieceValue, std::string colour, char symbol, Board *board):
   coord(x, y), pieceValue(pieceValue), colour(colour), symbol(symbol), board(board), moves(new std::vector<Coord>){}

Piece::~Piece(){
	delete moves;
}

int Piece::getX(){
	return coord.getX();
}

int Piece::getY(){
	return coord.getY();
}

int Piece::getValue(){
	return pieceValue;
}

std::string Piece::getColour(){
	return colour;
}

char const Piece::getSymbol(){
	return symbol;
}

Board *Piece::getBoard(){
	return board;
}

std::string Piece::getWhite(){
	return white;
}

std::string Piece::getBlack(){
	return black;
}

int Piece::getPawnVal(){
	return Piece::pawnVal;
}

int Piece::getRookVal(){
	return Piece::rookVal;
}

int Piece::getQueenVal(){
	return Piece::queenVal;
}

int Piece::getKnightVal(){
	return Piece::knightVal;
}

int Piece::getKingVal(){
	return Piece::kingVal;
}

int Piece::getBishopVal(){
	return Piece::bishopVal;
}


//checks to see if the piece can capture another piece p
bool Piece::canCapture(Piece &p){

	if (p.getColour() == colour || (p.getX() == getX() && p.getY() == getY())){
		return false;
	}

	if (!getMoves()){
		listOfMoves();
	}

        for (int i = 0; i < getMoves()->size(); i++){
                if (moves->at(i).getX() == p.getX() && moves->at(i).getY() == p.getY()){
                        return true;
                }
        }
        return false;
}


void Piece::moveStats()
{
	return;
}

bool Piece::canCastle()
{
	return false;
}

bool Piece::inCheck()
{
	return false;
}

bool Piece::inCheckmate()
{
	return false;
}


//performs the move to newSpot and returns if it was a success
bool Piece::move(Coord newSpot){
	bool canMove = false;

	//loop through all possible moves to see if newSpot is there
	for (std::vector<Coord>::iterator i = moves->begin(); i != moves->end(); i++){
		if (i->getX() == newSpot.getX() && i->getY() == newSpot.getY()){
			canMove = true;
			break;
		}
	}

	//look for en passent
	Pawn *pawn = dynamic_cast<Pawn*>(this);
	if (pawn){
		if (canMove && (newSpot.getY() == getY() - 2 || newSpot.getY() == getY() + 2)){
			board->notify(this->coord, newSpot);
			coord.setCoord(newSpot);
			getBoard()->setEnPassant(getX(), colour == getWhite() ? getY() - 1 : getY() + 1);
			getBoard()->setEnPassantColour(colour);
		} else if (canMove && getBoard()->getEnPassant() && newSpot.getX() == getBoard()->getEnPassant()->getX() && newSpot.getY() == getBoard()->getEnPassant()->getY()){
			board->notify(this->coord, newSpot);
			coord.setCoord(newSpot);

                        if (getBoard()->getEnPassant()) { getBoard()->setEnPassant(-1, -1); }
                        if (getBoard()->getEnPassant()) { getBoard()->setEnPassantColour(""); }

			Coord c(getX(), colour == getWhite() ? getY() - 1 : getY() + 1);
			getBoard()->remove(c);
		} else {
	                if (getBoard()->getEnPassant()) { getBoard()->setEnPassant(-1, -1); }
       	        	if (getBoard()->getEnPassant()) { getBoard()->setEnPassantColour(""); }
	                if (canMove){
	                        board->notify(this->coord, newSpot);
	                        coord.setCoord(newSpot);
	                }

		}
	} else {
                if (getBoard()->getEnPassant()) { getBoard()->setEnPassant(-1, -1); }
                if (getBoard()->getEnPassant()) { getBoard()->setEnPassantColour(""); }


		if (canMove){ //make the move
			board->notify(this->coord, newSpot);
			coord.setCoord(newSpot);
		}
	}
	moveStats();
	return canMove;
}


//print the symbol
std::ostream &operator<<(std::ostream &out, Piece &p){
	out << p.getSymbol();
	return out;
}

std::vector<Coord> *Piece::getMoves(){
	return moves;
}


//check to see if another piece can take this one
bool Piece::canBeCaptured(){
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			Piece *p = board->getPiece(i, j);
			if (p && p->canCapture(*this)){
				return true;
			}
		}
	}
	return false;
}


//return a list of 'safe' moves
std::vector<Coord>* Piece::safeMoves(){
	Board *board = getBoard();
	std::string colour = getColour();
	char symbol = getSymbol();

	std::vector<Coord> *sMoves = new std::vector<Coord>;
	std::vector<Coord> *moves = getMoves();

	Coord c(getX(), getY());
	board->pretend(c);

	for (int i = 0; i < moves->size(); i++){
		if (symbol == 'Q' || symbol == 'q'){
			Queen q(moves->at(i).getX(), moves->at(i).getY(), colour, board);
			if (!q.canBeCaptured()){
				sMoves->push_back(*(new Coord(moves->at(i).getX(), moves->at(i).getY())));
			}
		} else if (symbol == 'R' || symbol == 'r'){
                        Rook r(moves->at(i).getX(), moves->at(i).getY(), colour, board);
                        if (!r.canBeCaptured()){
                                sMoves->push_back(*(new Coord(moves->at(i).getX(), moves->at(i).getY())));
                        }
                } else if (symbol == 'B' || symbol == 'b'){
                        Bishop b(moves->at(i).getX(), moves->at(i).getY(), colour, board);
                        if (!b.canBeCaptured()){
                                sMoves->push_back(*(new Coord(moves->at(i).getX(), moves->at(i).getY())));
                        }
                } else if (symbol == 'N' || symbol == 'n'){
                        Knight n(moves->at(i).getX(), moves->at(i).getY(), colour, board);
                        if (!n.canBeCaptured()){
                                sMoves->push_back(*(new Coord(moves->at(i).getX(), moves->at(i).getY())));
                        }
                } else if (symbol == 'P' || symbol == 'p'){
                        Pawn p(moves->at(i).getX(), moves->at(i).getY(), colour, board);
                        if (!p.canBeCaptured()){
                                sMoves->push_back(*(new Coord(moves->at(i).getX(), moves->at(i).getY())));
                        }
                }
	}

	board->add(*this);
	return sMoves;
}
