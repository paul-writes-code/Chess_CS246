#ifndef __KING_H__
#define __KING_H__

#include "piece.h"
#include <vector>
#include <iostream>

class King: public Piece {
	bool firstMoveMade;
	void moveStats();
public:
	King(int x, int y, std::string colour, Board *b);
	~King();
	void listOfMoves();
//	bool canCapture(Piece &p);
	bool move(Coord newSpot);
	bool inCheck();
	bool inCheckmate();
};

#endif
