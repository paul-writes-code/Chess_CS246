#include "coord.h"
#include <iostream>

Coord::Coord(int x, int y): x(x), y(y) {}

Coord::~Coord() {}

int Coord::getX(){
	return x;
}

int Coord::getY(){
	return y;
}


//set the corrdinates
void Coord::setCoord(int newX, int newY)
{
	x = newX;
	y = newY;
}


//set the coordinates
void Coord::setCoord(Coord newSpot){
	x = newSpot.getX();
	y = newSpot.getY();
}


//return i it is on the board
bool Coord::valid()
{
	return (x >= 0) && (y >= 0) && (x < 8) && (y < 8);
}
