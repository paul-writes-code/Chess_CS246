#include "king.h"
#include "piece.h"
#include <vector>
#include <iostream>
#include "board.h"

King::King(int x, int y, std::string colour, Board *b):
   firstMoveMade(false),
   Piece(x, y, getKingVal(), colour, colour == getWhite() ? 'K' : 'k', b){}

King::~King(){}


//compile the king's kist of moves
void King::listOfMoves(){


	int x = getX();
	int y = getY();

	std::string colour = getColour();
	Board *b = getBoard();


	if (this->moves){
		delete this->moves;
	}

	std::vector<Coord> *moves = new std::vector<Coord>;
	Piece *p;

	Coord c(x, y);

	board->pretend(c);

	//check each possible move and see if it would be in check if it were there
	if (x + 1 <= 7){
		p = board->getPiece(x + 1, y);
		if (!p || (p && p->getColour() != colour)){
			King k(x + 1, y, colour, b);
			if (!k.inCheck()) moves->push_back(*(new Coord(x + 1, y)));
		}
	}
	if (y + 1 <= 7){
		p = board->getPiece(x, y + 1);
		if (!p || (p && p->getColour() != colour)){
                        King k(x, y + 1, colour, b);
                        if (!k.inCheck()) moves->push_back(*(new Coord(x, y + 1)));
		}
	}
	if (x + 1 <= 7 && y + 1 <= 7){
		p = board->getPiece(x + 1, y + 1);
		if (!p || (p && p->getColour() != colour)){
                        King k(x + 1, y + 1, colour, b);
                        if (!k.inCheck()) moves->push_back(*(new Coord(x + 1, y + 1)));
		}
	}
	if (x - 1 >= 0){
		p = board->getPiece(x - 1, y);
		if (!p || (p && p->getColour() != colour)){
                        King k(x - 1, y, colour, b);
                        if (!k.inCheck()) moves->push_back(*(new Coord(x - 1, y)));

		}
	}
	if (x - 1 >= 0 && y + 1 <= 7){
		p = board->getPiece(x - 1, y + 1);
		if (p){
		}
		if (!p || (p && p->getColour() != colour)){
                        King k(x - 1, y + 1, colour, b);
                        if (!k.inCheck()){
				moves->push_back(*(new Coord(x - 1, y + 1)));
			} else {
			}
		}
	}
	if (y - 1 >= 0){
		p = board->getPiece(x, y - 1);
		if (!p || (p && p->getColour() != colour)){
                        King k(x, y - 1, colour, b);
                        if (!k.inCheck()) moves->push_back(*(new Coord(x, y - 1)));
		}
	}
	if (y - 1 >= 0 && x - 1 >= 0){
		p = board->getPiece(x - 1, y - 1);
		if (!p || (p && p->getColour() != colour)){
                        King k(x - 1, y - 1, colour, b);
                        if (!k.inCheck()) moves->push_back(*(new Coord(x - 1, y - 1)));
		}
	}
	if (y - 1 >= 0 && x + 1 <= 7){
		p = board->getPiece(x + 1, y - 1);
		if (!p || (p && p->getColour() != colour)){
                        King k(x + 1, y - 1, colour, b);
                        if (!k.inCheck()) moves->push_back(*(new Coord(x + 1, y - 1)));
		}
	}

	if (colour == Piece::getWhite()){
		b->setWhiteKing(this);
	} else if (colour == Piece::getBlack()){
		b->setBlackKing(this);
	}

	board->add(*this);
	this->moves = moves;
}


//try to perform a move and return if it is successful
bool King::move(Coord newSpot)
{
	bool canMove = false;
	int count = 0;
	//right castle
	if (!firstMoveMade && newSpot.getY() == getY() && newSpot.getX() == getX() + 2)
	{
		Coord c(getX() + 4, getY());
		Coord c2(getX() + 1, getY());
		Piece * p;
		if (c.valid())
		{
			p = board->getPiece(c);
			if (p && p->canCastle() && p->getColour() == getColour())
			{
                                canMove = board->move(c, c2);
			}
		}
		c.setCoord(getX() + 3, getY());
		if (!canMove && c.valid())
		{
			p = board->getPiece(c);
			if (p && p->canCastle()  && p->getColour() == getColour())
			{
				canMove = board->move(c, c2);
			}
		}
	}
	//left castle
	else if (!firstMoveMade && newSpot.getY() == getY() && newSpot.getX() == getX() - 2)
	{
		Coord c(getX() - 4, getY());
                Coord c2(getX() - 1, getY());
                Piece * p;
                if (c.valid())
                {
                        p = board->getPiece(c);
                        if (p && p->canCastle() && p->getColour() == getColour())
                        {
                                canMove = board->move(c, c2);
                        }
                }
                c.setCoord(getX() - 3, getY());
                if (!canMove && c.valid())
                {
                        p = board->getPiece(c);
                        if (p && p->canCastle() && p->getColour() == getColour())
                        {
                                canMove = board->move(c, c2);
                        }
                }

	}
	else
	{
		for (std::vector<Coord>::iterator i = moves->begin(); i != moves->end(); i++)
		{
			if (i->getX() == newSpot.getX() && i->getY() == newSpot.getY())
			{
				canMove = true;
				break;
			}
			count ++;
		}
	}

	if (canMove)
	{
		board->notify(this->coord, newSpot);
		coord.setCoord(newSpot);
	}
	
	moveStats();

	return canMove;
}


void King::moveStats()
{
	firstMoveMade = true;
	return;
}


//look to see if the king is in check
bool King::inCheck(){
	std::string colour = getColour();
	int x = getX();
	int y = getY();
	Piece *p;

	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			p = board->getPiece(i, j);

			if (p && p->getSymbol() != 'k' && p->getSymbol() != 'K'){
				p->listOfMoves();
				if (p->getSymbol() == 'p'){
					if ((x == p->getX() + 1 || x == p->getX() - 1) && y == p->getY() - 1){
						return true;
					}
				} else if (p->getSymbol() == 'P'){
					if ((x == p->getX() + 1 || x == p->getX() - 1) && y == p->getY() + 1){
						return true;
					}
				} else if (p->canCapture(*this)){
					return true;
				}
			}
		}
	}
	return false;
}


//see if the king is in checkmate
bool King::inCheckmate(){
	listOfMoves();
	if (getMoves()->size() == 0 && inCheck()){
		return true;
	} else {
		return false;
	}
}
