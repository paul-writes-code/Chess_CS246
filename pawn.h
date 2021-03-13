#ifndef __PAWN_H__
#define __PAWN_H__

#include "piece.h"
#include "coord.h"
#include <vector>

class Pawn: public Piece {
	bool firstMoveMade;
	bool jumpedTwo;

public:
	Pawn(int x, int y, std::string colour, Board *b);
	~Pawn();
	void listOfMoves();
//	bool canCapture(Piece &p);
	bool getFirstMoveMade();
	void moveStats();
};

#endif
