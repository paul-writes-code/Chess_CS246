#ifndef __PIECE_H__
#define __PIECE_H__

#include "board.h"
#include "coord.h"
#include <iostream>
#include <vector>

class Board;

class Piece {
	static std::string white;
	static std::string black;
	static const int pawnVal;
	static const int knightVal;
	static const int bishopVal;
	static const int rookVal;
	static const int queenVal;
	static const int kingVal;
	int pieceValue;
	std::string colour;
	char symbol;
protected:
	Coord coord;
	Board *board;
	std::vector<Coord> *moves;
	virtual void moveStats();

public:
	Piece(int x, int y, int pieceValue, std::string colour, char symbol, Board *board);
	~Piece();
	
	int getX();
	int getY();
	int getValue();
	std::string getColour();
	char const getSymbol();
	Board *getBoard();
	virtual bool canCastle();
	virtual bool inCheck();
	virtual bool inCheckmate();

	static std::string getWhite();
	static std::string getBlack();
	static int getPawnVal();
	static int getRookVal();
	static int getQueenVal();
	static int getKnightVal();
	static int getKingVal();
	static int getBishopVal();

	bool canBeCaptured();
	std::vector<Coord> *safeMoves();
	virtual void listOfMoves() = 0;
	virtual bool move(Coord newSpot);
	bool canCapture(Piece &piece);
	std::vector<Coord> *getMoves();
	friend std::ostream &operator<<(std::ostream &out, Piece &p);
};

#endif
