#ifndef __QUEEN_H__
#define __QUEEN_H__

#include "piece.h"
#include <vector>
#include <iostream>

class Queen: public Piece {
public:
	Queen(int x, int y, std::string colour, Board *b);
	void listOfMoves();
//	bool canCapture(Piece &p);
};

#endif
