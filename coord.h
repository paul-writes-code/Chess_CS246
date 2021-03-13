#ifndef __COORD_H__
#define __COORD_H__

class Coord {
	int x;
	int y;
public:
	Coord(int x, int y);
	~Coord();

	int getX();
	int getY();
	bool valid();
	void setCoord(int newX, int newY);
	void setCoord(Coord newSpot);
};

#endif
