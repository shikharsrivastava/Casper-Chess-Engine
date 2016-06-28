#define VALIDKNIGHTPOS(x,y,i) (ISVALID(y)) && (LEVELDIFF(x,y) == knightLevel[i])
#define	VALIDSLIDEPOS(x,y,z,i) (ISVALID(y)) && (LEVELDIFF(x,y) == z*slideLevel[i])

bool BOARD::isSafe(int pos,int clr)
{
	
	ASSERT(ISVALID(pos))
	ASSERT(clr == WHITE || clr == BLACK)
	int from,opp,c,piece;

	opp=clr^1;

		//Checking for knight attack
		for(int i=1;i<=8;i++)
		{
			from=pos+knightIncr[i];
			piece=cboard[from];
			if(VALIDKNIGHTPOS(pos,from,i) && (TYPE(piece) == KNIGHT) && (color[piece] == opp))
			return false;	
		}
		
		//Checking for vertical and horizontal attack;
		for(int i=1;i<=4;i++)
		{
			from=pos+slideIncr[i];
			c=1;
			while(VALIDSLIDEPOS(pos,from,c,i))
			{
				piece=cboard[from];
				if(piece != EMPTY)
				{
				if((color[piece] == opp) && ((TYPE(piece) == ROOK) || (TYPE(piece) == QUEEN)))
				return false;
				else 
				break;
				}
				from+=slideIncr[i];
				c++;
			}
		} 	
		
		//Checking for Diagonal attacks
		for(int i=5;i<=8;i++)
		{
			from=pos+slideIncr[i];
			c=1;
			while(VALIDSLIDEPOS(pos,from,c,i))
			{
				piece=cboard[from];
				if(piece != EMPTY)
				{
				if((color[piece] == opp) && ((TYPE(piece) == BISHOP) || (TYPE(piece) == QUEEN)))
				return false;
				else
				break;
				}
				from+=slideIncr[i];
				c++;
			}
		}
		
		//Checking King attack

		for(int i=1;i<=8;i++)
		{
			from=pos+slideIncr[i];
			piece=cboard[from];
			c=1;
			if(VALIDSLIDEPOS(pos,from,c,i) && (color[piece] == opp) && (TYPE(piece) == KING))
			return false;
		}
	
		//Checking Pawn attack	
		if(clr == WHITE)
		{

			if((VALIDSLIDEPOS(pos,(pos+9),1,5) && (cboard[pos+9] == BLACK_PAWN)))
			return false;
			else if((VALIDSLIDEPOS(pos,(pos+7),1,6) && (cboard[pos+7] == BLACK_PAWN)))
			return false;
		}
		else
		{
			if((VALIDSLIDEPOS(pos,(pos-9),1,7) && (cboard[pos-9] == WHITE_PAWN)))
			return false;
			else if((VALIDSLIDEPOS(pos,(pos-7),1,8) && (cboard[pos-7] == WHITE_PAWN)))
			return false;
		}

	return true;
} 

void BOARD::printMove(int m)
{
std::cout<<boardPos[FROM(m)]<<boardPos[TO(m)];
}

void BOARD::printMove(MOVE m)
{
	printPiece(cboard[FROM(m.move)]);
	std::cout<<boardPos[FROM(m.move)]<<boardPos[TO(m.move)]<<" ";
	if(CAP(m.move) != EMPTY) {
	std::cout<<" / Captured ";
	printPiece(CAP(m.move));
	}
	if(EP(m.move))
	std::cout<<" / En Peasent Capture ";
	if(CAST(m.move))
	std::cout<<" / Castling move ";
	if(PS(m.move))
	std::cout<<" / Pawn Start ";
	std::cout<<m.move<<std::endl;
}

void BOARD::printMoveList(MOVE_LIST& list)
{
	for(int i=0;i<list.count;i++)
	{
		std::cout<<"Move "<<i+1<<" : ";
		printMove(list.l[i]);
		printf("score : %d\n",list.l[i].score);
	}
}

void BOARD::quietMove(int move,MOVE_LIST& list)
{

	ASSERT(CAP(move) == EMPTY)
	ASSERT(cboard[TO(move)] == EMPTY)
	ASSERT(cboard[FROM(move)] != EMPTY)
	list.l[list.count].move=move;
	if(searchKillers[0][ply] == move)
	list.l[list.count].score=900000;
	else if(searchKillers[1][ply] == move)
	list.l[list.count].score=800000;
	else
	list.l[list.count].score=searchHistory[cboard[FROM(move)]][TO(move)];
	list.count+=1;
	return ;
}

void BOARD::captureMove(int move,MOVE_LIST& list)
{
	
	ASSERT(CAP(move) != EMPTY)
	ASSERT(cboard[TO(move)] != EMPTY)
	ASSERT(cboard[FROM(move)] != EMPTY)	

	list.l[list.count].move=move;
	list.l[list.count].score=mvvlva[cboard[TO(move)]][cboard[FROM(move)]]+1000000;
	list.count+=1;
	return ;
}

void BOARD::enpsMove(int move,MOVE_LIST& list)
{
	ASSERT(CAP(move) == EMPTY)
	ASSERT(cboard[TO(move)] == EMPTY)
	ASSERT(cboard[FROM(move)] != EMPTY)
	ASSERT(EP(move))
	list.l[list.count].move=move;
	list.l[list.count].score=105+1000000;
	list.count+=1;
	return ;
}

void BOARD::whitePawnCapture(int from,int to,MOVE_LIST& list)
{
	if(from/8 == 6)
	{
		captureMove(CREATEMOVE(from,to,cboard[to],WHITE_QUEEN,0),list);
		captureMove(CREATEMOVE(from,to,cboard[to],WHITE_BISHOP,0),list);
		captureMove(CREATEMOVE(from,to,cboard[to],WHITE_ROOK,0),list);
		captureMove(CREATEMOVE(from,to,cboard[to],WHITE_KNIGHT,0),list);
	}
	else
	captureMove(CREATEMOVE(from,to,cboard[to],EMPTY,0),list);
	
}

void BOARD::whitePawnQuiet(int from,int to,MOVE_LIST& list)
{
	if(from/8 == 6)
	{
		quietMove(CREATEMOVE(from,to,EMPTY,WHITE_QUEEN,0),list);
		quietMove(CREATEMOVE(from,to,EMPTY,WHITE_BISHOP,0),list);
		quietMove(CREATEMOVE(from,to,EMPTY,WHITE_ROOK,0),list);
		quietMove(CREATEMOVE(from,to,EMPTY,WHITE_KNIGHT,0),list);
	}
	else
	quietMove(CREATEMOVE(from,to,EMPTY,EMPTY,0),list);
}
void BOARD::whitePawnMove(MOVE_LIST& list)
{
	bb wP=board[WHITE_PAWN];
	int from,to;
	while(wP >0)
	{
		from=(int)log2(wP & -wP);
		wP-=wP & -wP;
	
		to=from+8;
		if(ISVALID(to) && cboard[to] == EMPTY)
		{
			whitePawnQuiet(from,to,list);
			if((from/8)==1 && cboard[to+8]==EMPTY)
				quietMove(CREATEMOVE(from,to+8,EMPTY,EMPTY,PSF),list);			
		}

		to=from+9;
		if(ISVALID(to) && LEVELDIFF(from,to) == -1 && color[cboard[to]] == BLACK)
		whitePawnCapture(from,to,list);
		
		if(ISVALID(to) && LEVELDIFF(from,to) == -1 && to == enps)
		enpsMove(CREATEMOVE(from,to,EMPTY,EMPTY,EPF),list);

		to=from+7;
		if(ISVALID(to) && LEVELDIFF(from,to) == -1 && color[cboard[to]] == BLACK)
		whitePawnCapture(from,to,list);
	
		if(ISVALID(to) && LEVELDIFF(from,to) == -1 && to == enps)
		enpsMove(CREATEMOVE(from,to,EMPTY,EMPTY,EPF),list);
		
		
	}
}




void BOARD::blackPawnCapture(int from,int to,MOVE_LIST& list)
{
	if(from/8 == 1)
	{
		captureMove(CREATEMOVE(from,to,cboard[to],BLACK_QUEEN,0),list);
		captureMove(CREATEMOVE(from,to,cboard[to],BLACK_BISHOP,0),list);
		captureMove(CREATEMOVE(from,to,cboard[to],BLACK_ROOK,0),list);
		captureMove(CREATEMOVE(from,to,cboard[to],BLACK_KNIGHT,0),list);
	}
	else
	captureMove(CREATEMOVE(from,to,cboard[to],EMPTY,0),list);
	
}

void BOARD::blackPawnQuiet(int from,int to,MOVE_LIST& list)
{
	if(from/8 == 1)
	{
		quietMove(CREATEMOVE(from,to,EMPTY,BLACK_QUEEN,0),list);
		quietMove(CREATEMOVE(from,to,EMPTY,BLACK_BISHOP,0),list);
		quietMove(CREATEMOVE(from,to,EMPTY,BLACK_ROOK,0),list);
		quietMove(CREATEMOVE(from,to,EMPTY,BLACK_KNIGHT,0),list);
	}
	else
	quietMove(CREATEMOVE(from,to,EMPTY,EMPTY,0),list);
}
void BOARD::blackPawnMove(MOVE_LIST& list)
{
	bb bP=board[BLACK_PAWN];
	int from,to;
	while(bP >0)
	{
		from=(int)log2(bP & -bP);

		bP-=bP & -bP;
		to=from-8;
		if(ISVALID(to) && cboard[to] == EMPTY)
		{
			blackPawnQuiet(from,to,list);
			if((from/8)==6 && cboard[to-8]==EMPTY)
				quietMove(CREATEMOVE(from,to-8,EMPTY,EMPTY,PSF),list);			
		}

		to=from-7;
		if(ISVALID(to) && LEVELDIFF(from,to) == 1 && color[cboard[to]] == WHITE)
		blackPawnCapture(from,to,list);
		
		if(ISVALID(to) && LEVELDIFF(from,to) == 1 && to == enps)
		enpsMove(CREATEMOVE(from,to,EMPTY,EMPTY,EPF),list);

		to=from-9;
		if(ISVALID(to) && LEVELDIFF(from,to) == 1 && color[cboard[to]] == WHITE)
		blackPawnCapture(from,to,list);
	
		if(ISVALID(to) && LEVELDIFF(from,to) == 1 && to == enps)
		enpsMove(CREATEMOVE(from,to,EMPTY,EMPTY,EPF),list);
		
		
	}
}

void BOARD::slideMove(int piece,MOVE_LIST& list)
{	
	ASSERT(TYPE(piece) == ROOK || TYPE(piece) == BISHOP || TYPE(piece) == QUEEN)
	int from,to,s=color[piece],st,end,c;
	bb p;
	p=board[piece];
	int type=TYPE(piece);
	if(type == BISHOP)
	{st=5,end=8;}
	else if(type == ROOK)
	{st=1,end=4;}
	else if(type == QUEEN)
	{st=1,end=8;}
	else
	{ASSERT(0);}
	
	
	while(p>0)
	{
		from=(int)log2((p & -p));
			
		p-=p&-p;
		for(int i=st;i<=end;i++)
		{
			to=from+slideIncr[i];
			c=1;
			while(VALIDSLIDEPOS(from,to,c,i))
			{
				ASSERT(ISVALID(from))
				ASSERT(ISVALID(to))
				if(cboard[to] == EMPTY)
				quietMove(CREATEMOVE(from,to,EMPTY,EMPTY,0),list);
				else
				{
					if(color[cboard[to]] != s)
					captureMove(CREATEMOVE(from,to,cboard[to],EMPTY,0),list);
					break;
				}
				to+=slideIncr[i];
				c++;
			}
		}
	}	
}

void BOARD::generateKingMove(int piece,MOVE_LIST& list)
{
	int to,from=(int)log2(board[piece] & -board[piece]);
	
	ASSERT(TYPE(piece) == KING)	

	for(int i=1;i<=8;i++)
	{
		to=from+slideIncr[i];
		
		if(VALIDSLIDEPOS(from,to,1,i))
		{
			ASSERT(ISVALID(from))
			ASSERT(ISVALID(to))
			if(cboard[to]== EMPTY)
			quietMove(CREATEMOVE(from,to,EMPTY,EMPTY,0),list);
			else if(color[cboard[to]] != color[piece])
			captureMove(CREATEMOVE(from,to,cboard[to],EMPTY,0),list);	
		}
	}

	if(color[piece]==WHITE)
	{
		if(castle & WKC)
		{
			if(cboard[5] == EMPTY && cboard[6] == EMPTY)
			{
				if(isSafe(5,WHITE) && isSafe(6,WHITE) && isSafe(4,WHITE))
				quietMove(CREATEMOVE(4,6,EMPTY,EMPTY,CAF),list);
			}
		}
		if(castle & WQC)
		{
			if(cboard[3] == EMPTY && cboard[2] == EMPTY && cboard[1] == EMPTY)
			{
				if(isSafe(3,WHITE) && isSafe(2,WHITE) && isSafe(4,WHITE))
				quietMove(CREATEMOVE(4,2,EMPTY,EMPTY,CAF),list);
			}
		}
	}
	else
	{
		if(castle & BKC)
		{
			if(cboard[61] == EMPTY && cboard[62] == EMPTY)
			{
				if(isSafe(61,BLACK) && isSafe(62,BLACK) && isSafe(60,BLACK))
				quietMove(CREATEMOVE(60,62,EMPTY,EMPTY,CAF),list);
			}
		}
		if(castle & BQC)
		{
			if(cboard[57] == EMPTY && cboard[58] == EMPTY && cboard[59] == EMPTY)
			{
				if(isSafe(58,BLACK) && isSafe(59,BLACK) &&isSafe(60,BLACK))
				quietMove(CREATEMOVE(60,58,EMPTY,EMPTY,CAF),list);
			}
		}
	}

}

void BOARD::generateKnightMove(int piece,MOVE_LIST& list)
{
	ASSERT(TYPE(piece) == KNIGHT)
	int to,from;
	bb k=board[piece];
	
	while(k>0)
	{
		from=(int)log2(k & -k);
		
		for(int i=1;i<=8;i++)
		{
			to=from+knightIncr[i];
			if(VALIDKNIGHTPOS(from,to,i))
			{
				ASSERT(ISVALID(from))
				ASSERT(ISVALID(to))
				if(cboard[to]==EMPTY)
				quietMove(CREATEMOVE(from,to,EMPTY,EMPTY,0),list);
				else if(color[cboard[to]] != color[piece])
				captureMove(CREATEMOVE(from,to,cboard[to],EMPTY,0),list);			
			}	
		}
		k-=k&-k;
	}
}

void BOARD::generateAllMoves(MOVE_LIST& list)
{
	
	if(side == BLACK)
	{
		blackPawnMove(list);
		slideMove(BLACK_BISHOP,list);
		slideMove(BLACK_ROOK,list);
		slideMove(BLACK_QUEEN,list);
		generateKnightMove(BLACK_KNIGHT,list);
		generateKingMove(BLACK_KING,list);
		
	}
	else
	{
		
		whitePawnMove(list);
		slideMove(WHITE_BISHOP,list);
		slideMove(WHITE_ROOK,list);
		slideMove(WHITE_QUEEN,list);
		generateKnightMove(WHITE_KNIGHT,list);
		generateKingMove(WHITE_KING,list);		
	}
}

void BOARD::generateAllCap(MOVE_LIST &list)
{
	int from,to,piece,s,st,end,c,type;
	bb p;
	if(side == BLACK)
	{
	//pawn
	bb bP=board[BLACK_PAWN];
	
	while(bP >0)
	{
		from=(int)log2(bP & -bP);
		bP-=bP & -bP;
		to=from-7;
		if(ISVALID(to) && LEVELDIFF(from,to) == 1 && color[cboard[to]] == WHITE)
		blackPawnCapture(from,to,list);
		
		if(ISVALID(to) && LEVELDIFF(from,to) == 1 && to == enps)
		enpsMove(CREATEMOVE(from,to,EMPTY,EMPTY,EPF),list);

		to=from-9;
		if(ISVALID(to) && LEVELDIFF(from,to) == 1 && color[cboard[to]] == WHITE)
		blackPawnCapture(from,to,list);
	
		if(ISVALID(to) && LEVELDIFF(from,to) == 1 && to == enps)
		enpsMove(CREATEMOVE(from,to,EMPTY,EMPTY,EPF),list);
	}
	
	//BLACK bishop,rook,queen
	int parr[3]={BLACK_BISHOP,BLACK_ROOK,BLACK_QUEEN};
	int start[3]={5,1,1};
	int ending[3]={8,4,8};
	
	for(int idx=0;idx<3;idx++)
	{
	piece=parr[idx];
	s=BLACK;
	st=start[idx];
	end=ending[idx];
	p=board[piece];
	type=TYPE(piece);
	while(p>0)
	{
		from=(int)log2((p & -p));
		p-=p&-p;
		for(int i=st;i<=end;i++)
		{
			to=from+slideIncr[i];
			c=1;
			while(VALIDSLIDEPOS(from,to,c,i))
			{
				ASSERT(ISVALID(from))
				ASSERT(ISVALID(to))
				if(cboard[to] != EMPTY)
				{
					if(color[cboard[to]] == WHITE)
					captureMove(CREATEMOVE(from,to,cboard[to],EMPTY,0),list);
					break;
				}
				to+=slideIncr[i];
				c++;
			}
		}
	}	
	}

	//black knight
	piece=BLACK_KNIGHT;
	bb k=board[piece];
	while(k>0)
	{
		from=(int)log2(k & -k);
		
		for(int i=1;i<=8;i++)
		{
			to=from+knightIncr[i];
			if(VALIDKNIGHTPOS(from,to,i))
			{
				ASSERT(ISVALID(from))
				ASSERT(ISVALID(to))
				if(color[cboard[to]] == WHITE)
				captureMove(CREATEMOVE(from,to,cboard[to],EMPTY,0),list);			
			}	
		}
		k-=k&-k;
	}

	//black king
	piece=BLACK_KING;
	from=(int)log2(board[piece] & -board[piece]);
	ASSERT(TYPE(piece) == KING)	

	for(int i=1;i<=8;i++)
	{
		to=from+slideIncr[i];
		if(VALIDSLIDEPOS(from,to,1,i))
		{
			ASSERT(ISVALID(from))
			ASSERT(ISVALID(to))
			if(color[cboard[to]] == WHITE)
			captureMove(CREATEMOVE(from,to,cboard[to],EMPTY,0),list);	
		}
	}

	
	}
	else
	{

	bb wP=board[WHITE_PAWN];
	while(wP >0)
	{
		from=(int)log2(wP & -wP);
		wP-=wP & -wP;
	
		to=from+9;
		if(ISVALID(to) && LEVELDIFF(from,to) == -1 && color[cboard[to]] == BLACK)
		whitePawnCapture(from,to,list);
		
		if(ISVALID(to) && LEVELDIFF(from,to) == -1 && to == enps)
		enpsMove(CREATEMOVE(from,to,EMPTY,EMPTY,EPF),list);

		to=from+7;
		if(ISVALID(to) && LEVELDIFF(from,to) == -1 && color[cboard[to]] == BLACK)
		whitePawnCapture(from,to,list);
	
		if(ISVALID(to) && LEVELDIFF(from,to) == -1 && to == enps)
		enpsMove(CREATEMOVE(from,to,EMPTY,EMPTY,EPF),list);
		
	}
	

	int parr[3]={WHITE_BISHOP,WHITE_ROOK,WHITE_QUEEN};
	int start[3]={5,1,1};
	int ending[3]={8,4,8};

	for(int idx=0;idx<3;idx++)
	{
	piece=parr[idx];
	s=WHITE;
	st=start[idx];
	end=ending[idx];
	p=board[piece];
	type=TYPE(piece);
	while(p>0)
	{
		from=(int)log2((p & -p));
		p-=p&-p;
		for(int i=st;i<=end;i++)
		{
			to=from+slideIncr[i];
			c=1;
			while(VALIDSLIDEPOS(from,to,c,i))
			{
				ASSERT(ISVALID(from))
				ASSERT(ISVALID(to))
				if(cboard[to] != EMPTY)
				{
					if(color[cboard[to]] ==BLACK)
					captureMove(CREATEMOVE(from,to,cboard[to],EMPTY,0),list);
					break;
				}
				to+=slideIncr[i];
				c++;
			}
		}
	}	
	}
	//white knight
	piece=WHITE_KNIGHT;
	bb k=board[piece];
	while(k>0)
	{
		from=(int)log2(k & -k);
		
		for(int i=1;i<=8;i++)
		{
			to=from+knightIncr[i];
			if(VALIDKNIGHTPOS(from,to,i))
			{
				ASSERT(ISVALID(from))
				ASSERT(ISVALID(to))
				if(color[cboard[to]] == BLACK)
				captureMove(CREATEMOVE(from,to,cboard[to],EMPTY,0),list);			
			}	
		}
		k-=k&-k;
	}

	// white king
	piece=WHITE_KING;
	from=(int)log2(board[piece] & -board[piece]);
	ASSERT(TYPE(piece) == KING)	

	for(int i=1;i<=8;i++)
	{
		to=from+slideIncr[i];
		if(VALIDSLIDEPOS(from,to,1,i))
		{
			ASSERT(ISVALID(from))
			ASSERT(ISVALID(to))
			if(color[cboard[to]] == BLACK)
			captureMove(CREATEMOVE(from,to,cboard[to],EMPTY,0),list);	
		}
	}
	
	}
}


