#include <iostream>
#include "piece.h"
#include "coord.h"
#include "board.h"
#include <cstdlib>
#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"

using namespace std;


// a function used to turn symbols(characters) into strings for display

string Board::getString(char c)
{
	if (c == 'k' || c == 'K') return "King";
	if (c == 'q' || c == 'Q') return "Queen";
	if (c == 'b' || c == 'B') return "Bishop";
	if (c == 'n' || c == 'N') return "Knight";
	if (c == 'r' || c == 'R') return "Rook";
	if (c == 'p' || c == 'P') return "Pawn";
	if (c == 'A' - 1) c = 'B';
	
	string s("");
	s += c;

	return s;
}

// function to interprate colours

string Board::getString(string str){
	if (str == Piece::getWhite()) return "White";
	if (str == Piece::getBlack()) return "Black";

	return str;
}


// the inital pointer to the singleton board
Board * Board::board = NULL;


//draw the graphics, useful for testing
const bool drawGraphics = true;
//const bool drawGraphics = false;


//change the colour scheme
int background = Xwindow::Green;
int backgroundLight = Xwindow::Cyan;
int backgroundDark = Xwindow::Yellow;
int whiteColour = Xwindow::White;
int blackColour = Xwindow::Black;
int barrierColour = Xwindow::Blue;
int width = 560;


//board ctor
Board::Board()
{
	xwin = new Xwindow(9 * width / 8, 9 * width / 8);
	won = false;
	gameInProgress = false;
	blackStalemate = false;
	whiteStalemate = false;
	beenSetup = false;
	enPassant = NULL;
	enPassantColour = "";
	whiteKing = NULL;
	blackKing = NULL;

	//draw the boxes
	xwin->fillRectangle(0, 0, 9 * width / 8, 9 * width / 8, background);
	for (int i = 0; i < 8; i ++)
	{
		for (int j = 0; j < 8; j ++)
		{
			grid[i][j] = NULL;
			if (drawGraphics) xwin->fillRectangle(1 + i * width / 8, 1 + j * width / 8, width / 8, width / 8, ((i + j) % 2 == 1) ? backgroundLight : backgroundDark);
		}
	}
	//draw the coords along the sides
	for (int i = 0; i < 8 && drawGraphics; i ++)
	{
		xwin->drawString(width / 16 + i * width / 8, 17 * width / 16, getString('A' + (i == 1 ? -1 : i)));
		xwin->drawString(17 * width / 16, width / 16 + (7 - i) * width / 8, getString('1' + i));
	}
	//draw the lines
	for (int i = width / 8; i <= width && drawGraphics; i += width / 8)
	{
        	xwin->fillRectangle(i, 0, 1, 9 * width / 8, barrierColour);
		xwin->fillRectangle(0, i, 9 * width / 8, 1, barrierColour);
	}
}

Coord *Board::getEnPassant(){
	return enPassant;
}

void Board::setEnPassant(int x, int y){
	delete enPassant;
	enPassant = x < 0 ? 0 : new Coord(x, y);
}

std::string Board::getEnPassantColour(){
	return enPassantColour;
}

void Board::setEnPassantColour(std::string newColour){
	enPassantColour = newColour;
}

void Board::setWhiteKing(Piece *k){
	whiteKing = k;
}

void Board::setBlackKing(Piece *k){
	blackKing = k;
}


//returns the colour of the team in check ("" if none)
std::string Board::inCheck(){
	if (whiteKing && whiteKing->inCheck()){
		return Piece::getWhite();
	} else if (blackKing && blackKing->inCheck()){
		return Piece::getBlack();
	} else {
		return "";
	}
}


//returns the colour of the team in checkmate ("" if none)
std::string Board::inCheckmate(){

	if (whiteKing && whiteKing->inCheckmate()){
		return Piece::getWhite();
	} else if (blackKing && blackKing->inCheckmate()){

		return Piece::getBlack();
	} else {
		return "";
	}
	gameInProgress = false;
}


//refreshes all the pieces moves
void Board::checkAllMoves()
{
	for (int i = 0; i < 8; i ++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (grid[i][j] && grid[i][j]->getSymbol() != 'K' && grid[i][j]->getSymbol() != 'k') {
				grid[i][j]->listOfMoves();
			}
		}
	}
	if (whiteKing) whiteKing->listOfMoves();
	if (blackKing) blackKing->listOfMoves();
}


//determines if a board is valid
bool Board::validBoard()
{
	if (!gameInProgress)
	{
		int K = 0;
		int k = 0;

		int whiteX = -1;
		int whiteY = -1;
		int blackX = -1;
		int blackY = -1;

		//see if there are pawns at the ends
		for (int i = 0; i < 8; i++){
			Piece *p = grid[i][0];
			if (p && (p->getSymbol() == 'p' || p->getSymbol() == 'P')) return false;		
			p = grid[i][7];
			if (p && (p->getSymbol() == 'p' || p->getSymbol() == 'P')) return false;
		}


		//check that there are only 2 kings
		for (int i = 0; i < 8; i ++)
		{
			for (int j = 0; j < 8; j ++)
			{
				if (grid[i][j])
				{
					if (grid[i][j]->getSymbol() == 'K'){ 
						K ++;
						whiteX = i;
						whiteY = j;
					}
					if (grid[i][j]->getSymbol() == 'k'){ 
						k ++;
						blackX = i;
						blackY = j;
					}
				}
			}
		}
	
		if (K != 1 || k != 1) return false;
	
		King wKing(whiteX, whiteY, Piece::getWhite(), this);
		King bKing(blackX, blackY, Piece::getBlack(), this);
	
		if (!gameInProgress) return !wKing.inCheck() && !bKing.inCheck();
	}
	return (whiteKing && blackKing);
}


//function telling the board that something has changed at start and end
void Board::notify(Coord start, Coord end)
{
	if (grid[end.getX()][end.getY()] != NULL)
	{
		if (grid[end.getX()][end.getY()]->getSymbol() == 'k') blackKing = NULL;
                if (grid[end.getX()][end.getY()]->getSymbol() == 'K') whiteKing = NULL;
		delete grid[end.getX()][end.getY()];
	}	
	grid[end.getX()][end.getY()] = grid[start.getX()][start.getY()];
	grid[start.getX()][start.getY()] = NULL;


	//update the graphics
	if (drawGraphics)
	{
	//end

		xwin->fillRectangle((end.getX() * width / 8) + 1, ((7 - end.getY()) * width / 8) + 1, (width / 8) - 1, (width / 8) - 1, (getPiece(end)->getColour() == Piece::getWhite()) ? whiteColour : blackColour);
        	xwin->drawString((end.getX() * width / 8) + (width / 32), ((7 - end.getY()) * width / 8) + (width / 16), getString(getPiece(end)->getSymbol()), barrierColour);

	//start

		xwin->fillRectangle((start.getX() * width / 8) + 1, ((7 - start.getY()) * width / 8) + 1, (width / 8) - 1, (width / 8) - 1, ((start.getX() + start.getY()) % 2 == 1) ? backgroundDark : backgroundLight);
	}


	//check if a king has been taken
	won = !(this->validBoard());
}

void cleanUp()
{
	Board * b = Board::getInstance();
	delete b;
}


//returns a pointer to the board
Board * Board::getInstance()
{
	if (!board)
	{
		board = new Board;
		atexit(cleanUp);
	}
	return board;
}

Board::~Board()
{
	this->clearBoard();
	delete xwin;
}

void Board::setup()
{
	beenSetup = true;
}


//checks if a move is valid
bool Board::legalMove(Coord start, Coord end)
{
	char orig = 'x';
	char final = 'x';
	if (grid[start.getX()][start.getY()])
	{
		orig = grid[start.getX()][start.getY()]->getSymbol();
	}
        if (grid[end.getX()][end.getY()])
        {
                final = grid[end.getX()][end.getY()]->getSymbol();
        }
	if (orig == 'x') return false;
	if (final == 'x') return true;
	if (orig <= 'z' && orig >= 'a') return ((final <= 'Z') && (final >= 'A'));
	if (orig <= 'Z' && orig >= 'A') return ((final <= 'z') && (final >= 'a'));
	return false;
}


//puts a new piece on the board (removes the old if it were there)
void Board::add(Piece & p)
{
	if (grid[p.getX()][p.getY()])
	{
		delete (this->grid)[p.getX()][p.getY()];
	}
	grid[p.getX()][p.getY()] = &p;
	

	//refresh the graphics
	if (drawGraphics)
	{
		xwin->fillRectangle((p.getX() * width / 8) + 1, ((7 - p.getY()) * width / 8) + 1, (width / 8) - 1, (width / 8) - 1, (p.getColour() == Piece::getWhite()) ? whiteColour : blackColour);
		xwin->drawString((p.getX() * width / 8) + (width / 32), ((7 - p.getY()) * width / 8) + (width / 16), getString(p.getSymbol()), barrierColour);
	}

	if (p.getSymbol() == 'K'){
		setWhiteKing(&p);
	} else if (p.getSymbol() == 'k'){
		setBlackKing(&p);
	}
}


//pretend a piece is not there (does not delete)
void Board::pretend(Coord c)
{
	if (getPiece(c)->getSymbol() == 'K'){
		whiteKing = NULL;
	} else if (getPiece(c)->getSymbol() == 'k'){
		blackKing = NULL;
	}	

	grid[c.getX()][c.getY()] = NULL;
}


//deletes a piece from the board
void Board::remove(Coord c)
{
	if (getPiece(c)->getSymbol() == 'K'){
		whiteKing = NULL;
	} else if (getPiece(c)->getSymbol() == 'k'){
		blackKing = NULL;
	}
	delete grid[c.getX()][c.getY()];
	grid[c.getX()][c.getY()] = NULL;
	if (drawGraphics) xwin->fillRectangle((c.getX() * width / 8) + 1, ((7 - c.getY()) * width / 8) + 1, (width / 8) - 1, (width / 8) - 1, ((c.getX() + c.getY()) % 2 == 0) ? backgroundLight : backgroundDark);
}


bool Board::isWon()
{
	return won;
}

//checks if there is a stalemate

bool Board::isStalemate()
{
        blackStalemate = true;
        whiteStalemate = true;
        for (int i = 0; i < 8; i ++) //loop through all moves
        {
                for (int j = 0; j < 8; j ++)
                {
                        if (grid[i][j])
                        {
                                grid[i][j]->listOfMoves();
                                if (!(grid[i][j]->getMoves()->empty())) //a piece can move
                                {
                                        if (grid[i][j]->getSymbol() >= 'a' && grid[i][j]->getSymbol() <= 'z')
                                        {
                                                blackStalemate = false;
                                        }
                                        if (grid[i][j]->getSymbol() >= 'A' && grid[i][j]->getSymbol() <= 'Z')
                                        {
                                                whiteStalemate = false;
                                        }
                                }
                        }
                }
        }

	return blackStalemate || whiteStalemate;

//deletes all pieces on the board
}

void Board::clearBoard()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j ++)
		{
			if (drawGraphics) xwin->fillRectangle(1 + i * width / 8, 1 + j * width / 8, width / 8, width / 8, ((i + j) % 2 == 1) ? backgroundLight : backgroundDark);
			delete grid[i][j];
			grid[i][j] = NULL;
		}
	}
        for (int i = width / 8; i < 9 * width / 8 && drawGraphics; i += width / 8)
        {
                xwin->fillRectangle(i, 0, 1, width, barrierColour);
                xwin->fillRectangle(0, i, width, 1, barrierColour);
        }
}


//tries to make a move (start -> end) and returns if it is successful
bool Board::move(Coord start, Coord end)
{
	bool ret = false;
	if (start.getX() >= 0 && start.getX() <= 7 && start.getY() >= 0 && start.getY() <= 7 &&
	    end.getX() >= 0 && end.getX() <= 7 && end.getY() >= 0 && end.getY() <= 7)
	{
		ret =  grid[start.getX()][start.getY()]->move(end);
		checkAllMoves();
	}
	return ret;
}


//start a new game
void Board::start()
{
	if (!beenSetup)
	{
		clearBoard();
		for (int i = 0; i < 8; i ++)
		{
			this->add(*(new Pawn(i, 1, Piece::getWhite(), this)));
			this->add(*(new Pawn(i, 6, Piece::getBlack(), this)));
		}
		this->add(*(new Rook(0, 0, Piece::getWhite(),this)));
                this->add(*(new Rook(7, 0, Piece::getWhite(),this)));
                this->add(*(new Rook(0, 7, Piece::getBlack(),this)));
                this->add(*(new Rook(7, 7, Piece::getBlack(),this)));

		this->add(*(new Knight(1, 0, Piece::getWhite(),this)));
                this->add(*(new Knight(6, 0, Piece::getWhite(),this)));
                this->add(*(new Knight(1, 7, Piece::getBlack(),this)));
                this->add(*(new Knight(6, 7, Piece::getBlack(),this)));

                this->add(*(new Bishop(2, 0, Piece::getWhite(),this)));
                this->add(*(new Bishop(5, 0, Piece::getWhite(),this)));
                this->add(*(new Bishop(2, 7, Piece::getBlack(),this)));
                this->add(*(new Bishop(5, 7, Piece::getBlack(),this)));

                this->add(*(new Queen(3, 0, Piece::getWhite(),this)));
		this->add(*(new King(4, 0, Piece::getWhite(), this)));

                this->add(*(new Queen(3, 7, Piece::getBlack(),this)));
		this->add(*(new King(4, 7, Piece::getBlack(), this)));
	}

	checkAllMoves();
	gameInProgress = true;
	beenSetup = false;
	won = false;
}

//prints the board to out
std::ostream & operator<<(std::ostream & out, const Board & board)
{
	for (int i = 7; i >= 0; i --)
	{
		out << (i + 1) << ' ';
		for (int j = 0; j < 8; j++)
		{
			if (board.grid[j][i])
			{
				out << *(board.grid[j][i]);
			}
			else if ((i + j) % 2 == 0)
			{
				out << '_';
			}
			else
			{
				out << ' ';
			}
		}
		out << '\n';
	}
	out << '\n' << "  ";
	for (char i = 'a'; i < 'i'; i ++)
	{
		out << i;
	}
	out << '\n';
}

Piece * Board::getPiece(Coord c)
{
	return grid[c.getX()][c.getY()];
}

Piece * Board::getPiece(int x, int y)
{
	return grid[x][y];
}

Piece * Board::getKing(string colour)
{
	if (colour == Piece::getWhite())
	{
		return whiteKing;
	}
	else
	{
		return blackKing;
	}
}
