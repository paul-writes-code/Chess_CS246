#ifndef __ROOK_H__
#define __ROOK_H__

#include "piece.h"
#include <vector>
#include <iostream>

class Rook: public Piece {
	bool firstMoveMade;
	void moveStats();
public:
	Rook(int x, int y, std::string colour, Board *b);
	~Rook();
	void listOfMoves();
//	bool canCapture(Piece &p);
	bool canCastle();
};

#endif
