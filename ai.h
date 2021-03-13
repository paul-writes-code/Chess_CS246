#include "board.h"
#include "piece.h"
#include "king.h"
#include "queen.h"
#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "coord.h"
#include <cstdlib>
#include <time.h>
#include <iostream>

//void aiMove1(Board * b, std::string colour);

//void aiMove2(Board * b, std::string colour);

void aiMove(int level, Board * b, std::string colour);
