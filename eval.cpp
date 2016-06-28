const int pawnTable[64] = {
0   ,   0   ,   0   ,   0   ,   0   ,   0   ,   0   ,   0   ,
10  ,   10  ,   0   ,  -10  ,   -10 ,   0   ,   10  ,   10  ,
5   ,   0   ,   0   ,   5   ,   5   ,   0   ,   0   ,   5   ,
0   ,   0   ,   10  ,   20  ,   20  ,   10  ,   0   ,   0   ,
5   ,   5   ,   5   ,   10  ,   10  ,   5   ,   5   ,   5   ,
10  ,   10  ,   10  ,   20  ,   20  ,   10  ,   10  ,   10  ,
20  ,   20  ,   20  ,   30  ,   30  ,   20  ,   20  ,   20   ,
0   ,   0   ,   0   ,   0   ,   0   ,   0   ,   0   ,   0   ,
};

const int knightTable[64] = {
0   ,   -10 ,   0   ,   0   ,   0   ,   0   ,   -10 ,   0   ,
0   ,   0   ,   0   ,   5   ,   5   ,   0   ,   0   ,   0   ,
0   ,   0   ,   10  ,   10  ,   10  ,   10  ,   0   ,   0   ,
0   ,   0   ,   10  ,   20  ,   20  ,   10  ,   5   ,   0   ,
5   ,   10  ,   15  ,   20  ,   20  ,   15  ,   10  ,   5   ,
5   ,   10  ,   10  ,   20  ,   20  ,   10  ,   10  ,   5   ,
0   ,   0   ,   5   ,   10  ,   10  ,   5   ,   0   ,   0   ,
0   ,   0   ,   0   ,   0   ,   0   ,   0   ,   0   ,   0   ,
};

const int bishopTable[64] = {
0   ,   0   ,   -10 ,   0   ,   0   ,   -10 ,   0   ,   0   ,
0   ,   0   ,   0   ,   10  ,   10  ,   0   ,   0   ,   0   ,
0   ,   0   ,   10  ,   15  ,   15  ,   10  ,   0   ,   0   ,
0   ,   10  ,   15  ,   20  ,   20  ,   15  ,   10  ,   0   ,
0   ,   10  ,   15  ,   20  ,   20  ,   15  ,   10  ,   0   ,
0   ,   0   ,   10  ,   15  ,   15  ,   10  ,   0   ,   0   ,
0   ,   0   ,   0   ,   10  ,   10  ,   0   ,   0   ,   0   ,
0   ,   0   ,   0   ,   0   ,   0   ,   0   ,   0   ,   0   ,
};

const int rookTable[64] = {
0   ,   0   ,   5   ,   10  ,   10  ,   5   ,   0   ,   0   ,
0   ,   0   ,   5   ,   10  ,   10  ,   5   ,   0   ,   0   ,
0   ,   0   ,   5   ,   10  ,   10  ,   5   ,   0   ,   0   ,
0   ,   0   ,   5   ,   10  ,   10  ,   5   ,   0   ,   0   ,
0   ,   0   ,   5   ,   10  ,   10  ,   5   ,   0   ,   0   ,
0   ,   0   ,   5   ,   10  ,   10  ,   5   ,   0   ,   0   ,
25  ,   25  ,   25  ,   25  ,   25  ,   25  ,   25  ,   25  ,
0   ,   0   ,   5   ,   10  ,   10  ,   5   ,   0   ,   0   ,
};

const int mirror[64] = {
56  ,   57  ,   58  ,   59  ,   60  ,   61  ,   62   ,   63   ,
48  ,   49  ,   50  ,   51  ,   52  ,   53  ,   54   ,   55   ,
40  ,   41  ,   42  ,   43  ,   44  ,   45  ,   46   ,   47   ,
32  ,   33  ,   34  ,   35  ,   36  ,   37  ,   38   ,   39   ,
24  ,   25  ,   26  ,   27  ,   28  ,   29  ,   30   ,   31   ,
16  ,   17  ,   18  ,   19  ,   20  ,   21  ,   22   ,   23   ,
8   ,   9   ,   10  ,   11  ,   12  ,   13  ,   14   ,   15   ,
0   ,   1   ,   2   ,   3   ,   4   ,   5   ,   6    ,   7    ,
};

int BOARD::evaluate()
{
	int score=material[WHITE]-material[BLACK];
	int pos;
	bb p;

	p=board[WHITE_PAWN];
	while(p>0)
	{
		pos=(int)log2(p &-p);
		p-=p&-p;
		score+=pawnTable[pos];
	}
	
	p=board[BLACK_PAWN];
	while(p>0)
	{
		pos=(int)log2(p &-p);
		p-=p&-p;
		score-=pawnTable[mirror[pos]];
	}

	p=board[WHITE_KNIGHT];
	while(p>0)
	{
		pos=(int)log2(p &-p);
		p-=p&-p;
		score+=knightTable[pos];
	}

	p=board[BLACK_KNIGHT];
	while(p>0)
	{
		pos=(int)log2(p &-p);
		p-=p&-p;
		score-=knightTable[mirror[pos]];
	}

	p=board[WHITE_ROOK];
	while(p>0)
	{
		pos=(int)log2(p &-p);
		p-=p&-p;
		score+=rookTable[pos];
	}

	p=board[BLACK_ROOK];
	while(p>0)
	{
		pos=(int)log2(p &-p);
		p-=p&-p;
		score-=rookTable[mirror[pos]];
	}

	p=board[WHITE_BISHOP];
	while(p>0)
	{
		pos=(int)log2(p &-p);
		p-=p&-p;
		score+=bishopTable[pos];
	}

	p=board[BLACK_BISHOP];
	while(p>0)
	{
		pos=(int)log2(p &-p);
		p-=p&-p;
		score-=bishopTable[mirror[pos]];
	}
	
	if(side == WHITE)
	return score;
	else
	return -score;
}
