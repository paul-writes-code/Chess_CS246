#ifndef __BOARD_H__
#define __BOARD_H__

#include <iostream>
#include <string>
#include "coord.h"
#include "window.h"

class Piece;
class King;

class Board
{
	//  Fields
	static Board * board;

	Xwindow * xwin;

	bool won;
	bool gameInProgress;
	bool blackStalemate;
	bool whiteStalemate;
	bool beenSetup;

	Coord *enPassant;
	std::string enPassantColour;

	Piece *whiteKing;
	Piece *blackKing;

	Piece * grid[8][8];

	//  Methods
	Board();
	void checkAllMoves();
	
	public:
	
	void notify(Coord start, Coord end);
	static Board * getInstance();
	
	~Board();
	bool validBoard();
	void setup();
	bool legalMove(Coord start, Coord end);
	void add(Piece & p);
	void pretend(Coord c);
	void remove(Coord c);
	bool canBeCaptured(Piece & p);
	bool isWon();
	bool isStalemate();
	void clearBoard();
	bool move(Coord start, Coord end);
	void start();
	Piece *getPiece(Coord c);
	Piece *getPiece(int x, int y);
	Coord *getEnPassant();
	void setEnPassant(int x, int y);
	std::string getEnPassantColour();
	void setEnPassantColour(std::string str);
	std::string inCheck();
	std::string inCheckmate();

	void setWhiteKing(Piece *k);
	void setBlackKing(Piece *k);
	Piece * getKing(std::string colour);

	std::string getString(char c);
	std::string getString(std::string str);

	friend std::ostream & operator<<(std::ostream & out, const Board & board);
};

#endif
