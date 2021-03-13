#ifndef __KNIGHT_H__
#define __KNIGHT_H__

#include "piece.h"
#include <iostream>
#include "coord.h"
#include <vector>

class Knight: public Piece {
public:
	Knight(int x, int y, std::string colour, Board *b);
	~Knight();
	void listOfMoves();
//	bool canCapture(Piece &p);
};

#endif
