#include "ai.h"

using namespace std;


//make the move
void perform(Coord start, Coord end, Board * b)
{
	if (!b->move(start, end))
        {
		//something is wrong if you can't move where you are trying to
                cout << "Something is wrong with AI" << endl;
        }
        else
        {
		char c = 'a' + end.getX();
                cout << "The AI has moved to " << c << end.getY() + 1 << endl;
                if ((b->getPiece(end)->getSymbol() == 'p' && end.getY() == 0) || (b->getPiece(end)->getSymbol() == 'P' && end.getY() == 7))
                {
                        b->add(*(new Queen(end.getX(), end.getY(), b->getPiece(end)->getColour(), b)));
                }
        }
}


//level 1 move (random)
void aiMove1(Board * b, std::string colour)
{
	srand(time(NULL));
	Piece * p = NULL;
	int x = rand() % 8, y = rand() % 8;
	while (!p || p->getColour() != colour || p->getMoves()->size() == 0)
	{
		p = b->getPiece(x, y);
		x = rand() % 8;
		y = rand() % 8;
	}
	vector<Coord> * list = p->getMoves();
	int m = rand() % list->size();
	Coord start(p->getX(), p->getY());
	perform(start, list->at(m), b);
}


//level 2 move (perefers checks and captures)
void aiMove2(Board * b, std::string colour)
{
	srand(time(NULL));
	vector<Piece *> plainList;
	// add all pieces of correct colour
	for (int i = 0; i < 8; i ++)
	{
		for (int j = 0; j < 8; j ++)
		{
			Coord c(i, j);
			Piece * p = b->getPiece(c);
			if (p && p->getColour() == colour)
			{
				plainList.push_back(p);
			}
		}
	}
	// form a new list with only pieces that can capture

	vector<Piece *> captureList;
	vector<Coord> captureEnd;
	for (int i = 0; i < 8; i ++)
        {
                for (int j = 0; j < 8; j ++)
                {
                        Piece * p = b->getPiece(i, j);
                        if (p && p->getColour() != colour)
                        {
                                for (int k = 0; k < plainList.size(); k ++)
				{
					if (plainList[k]->canCapture(*p))
					{
						captureList.push_back(plainList[k]);
						captureEnd.push_back(*(new Coord(i, j)));
					}
				}
                        }
                }
        }
	// form a check list
	vector<Piece *> checkList;
	vector<Coord> checkEnd;
	vector<Coord> * moves;

	Piece * king = b->getKing((colour == Piece::getWhite()) ? Piece::getBlack(): Piece::getWhite());
	for (int i = 0; i < plainList.size(); i ++)
	{
		moves = plainList[i]->getMoves();
		for (int j = 0; j < moves->size(); j ++)
		{

			Piece * p;

			if (plainList[i]->getSymbol() == 'p' || plainList[i]->getSymbol() == 'P')
			{
				p = new Pawn(moves->at(j).getX(), moves->at(j).getY(), colour, b);
			}
                        else if (plainList[i]->getSymbol() == 'r' || plainList[i]->getSymbol() == 'R')
                        {
				p = new Rook(moves->at(j).getX(), moves->at(j).getY(), colour, b);
			}
			else if (plainList[i]->getSymbol() == 'n' || plainList[i]->getSymbol() == 'N')
                        {
                                p = new Knight(moves->at(j).getX(), moves->at(j).getY(), colour, b);
			}
			else if (plainList[i]->getSymbol() == 'b' || plainList[i]->getSymbol() == 'B')
                        {
                                p = new Bishop(moves->at(j).getX(), moves->at(j).getY(), colour, b);
			}
			else if (plainList[i]->getSymbol() == 'q' || plainList[i]->getSymbol() == 'Q')
                        {
                                p = new Queen(moves->at(j).getX(), moves->at(j).getY(), colour, b);
			}
			else //king
			{
                                p = new King(moves->at(j).getX(), moves->at(j).getY(), colour, b);

			}

			if (p && p->canCapture(*king))
			{
				checkList.push_back(plainList[i]);
				checkEnd.push_back((*moves)[j]);
			}
			delete p;
		}
	}
	int pos;
	if (!(checkList.empty()))
	{
		pos = rand() % checkList.size();
		Coord start(checkList[pos]->getX(), checkList[pos]->getY());
		perform(start, checkEnd[pos], b);
	}
	else if (!(captureList.empty()))
	{
		pos = rand() % captureList.size();
                Coord start(captureList[pos]->getX(), captureList[pos]->getY());
                perform(start, captureEnd[pos], b);
	}
	else
	{
		aiMove1(b, colour);
	}
}


//level 3 move (perfers avoiding capture, checks and captures)
void aiMove3(Board * b, std::string colour)
{
	srand(time(NULL));
        vector<Piece *> plainList;
        // add all pieces of correct colour
        for (int i = 0; i < 8; i ++)
        {
                for (int j = 0; j < 8; j ++)
                {
                        Coord c(i, j);
                        Piece * p = b->getPiece(c);
                        if (p && p->getColour() == colour)
                        {
                                plainList.push_back(p);
                        }
                }
        }
        // form a new list with only pieces that can capture


	vector<Piece *> canBeCaptured;
	vector<Piece *> savable;

	for (int i = 0; i < plainList.size(); i++){
		if (plainList.at(i)->canBeCaptured()){
			canBeCaptured.push_back(plainList.at(i));
		}
	}


	for (int i = 0; i < canBeCaptured.size(); i++){
		vector<Coord> *sMoves = canBeCaptured.at(i)->safeMoves();

		if (sMoves->size() > 0){
			savable.push_back(canBeCaptured.at(i));
		}
		delete sMoves;
	}

	if (savable.size() > 0){
		int r = rand() % savable.size();

		Piece *save = savable.at(r); // this is the piece we will save
	
		vector<Coord> *sMoves = save->safeMoves();
		int m = rand() % sMoves->size();
	
		Coord start(save->getX(), save->getY());
		perform(start, sMoves->at(m), b);
	
		delete sMoves;
		return;
	}

	aiMove2(b, colour);
}


//perform a move of the corrcet level
void aiMove(int level, Board *b, std::string colour){
	if (level == 1){
		aiMove1(b, colour);
	} else if (level == 2){
		aiMove2(b, colour);
	} else if (level == 3 || level == 4){
		aiMove3(b, colour);
	}
}
