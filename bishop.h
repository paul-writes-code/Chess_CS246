#ifndef __BISHOP_H__
#define __BISHOP_H__

#include "piece.h"
#include <vector>
#include <iostream>

class Bishop: public Piece {
public:
	Bishop(int x, int y, std::string colour, Board *b);
	~Bishop();
	void listOfMoves();
//	bool canCapture(Piece &p);
};

#endif
