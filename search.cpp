static void pickBest(int id,MOVE_LIST &moveList)
{
	MOVE tmp;
	int ind=id,best=0;
	for(int i=id;i<moveList.count;i++)
	{
		if(moveList.l[i].score >best)
		{
			best=moveList.l[i].score;
			ind=i;
		}
	}


	tmp=moveList.l[id];
	moveList.l[id]=moveList.l[ind];
	moveList.l[ind]=tmp;	
}


bool BOARD::repeat()
{
	for(int i=hisply-fifty;i<hisply-1;i++)
	{
		if(poskey == his[i].poskey)
		return true;
	}
	return false;
}

int BOARD::getTime()
{
	return GetTickCount();
}

void BOARD::initPvTable()
{
	tab.entries=1000000/sizeof(PVENTRY);
	free(tab.table);
	tab.table=(PVENTRY*)calloc(tab.entries,sizeof(PVENTRY));

}

void BOARD::storePv(int move)
{
	int index=poskey%tab.entries;
	tab.table[index].poskey=poskey;
	tab.table[index].move=move;
}

int BOARD::getPvMove()
{
	int index=poskey%tab.entries;
	if(tab.table[index].poskey == poskey)
	return tab.table[index].move;	
	return NOMOVE;
}

int BOARD::getPvLine(int depth)
{
	int c=0;
	int move=getPvMove();
	
	while(move != NOMOVE && c<depth)
	{
		PVARRAY[c++]=move;
		makeMove(move);					//Not Checked if the move was legal
		move=getPvMove();	
	}
	
	while(ply>0)
	unmakeMove();
	
	return c;	
}

void BOARD::clearPv()
{
	for(int i=0;i<tab.entries;i++)
	{
		tab.table[i].move=0;
		tab.table[i].poskey=0;
	}
}


void BOARD::check()
{
	if(info.timeset && getTime() > info.stoptime)
	info.stopped=true;
	readInp();
}

void BOARD::clearForSearch()
{
	for(int i=0;i<TOTAL_BITBOARD;i++)
	{
		for(int j=0;j<64;j++)
		searchHistory[i][j]=0;	
	}
	
	for(int i=0;i<2;i++)
	{
		for(int j=0;j<MAXDEPTH;j++)
		searchKillers[i][j]=0;
	}
	clearPv();
	ply=0;
	info.stopped = false;
	info.quit=false;
	info.nodes = 0;
	info.fhf=0;
	info.fh=0;
}

void BOARD::iterativeDeepening()
{
	int bestscore=-INFINITE;
	int bestmove =NOMOVE;
	int numPv=0;

	clearForSearch();
	for(int i=1;i<=info.depth;i++)
	{
		bestscore=alphabeta(i,-INFINITE,INFINITE,true);
		if(info.stopped)
		break;
		numPv=getPvLine(i);
		bestmove=PVARRAY[0];
		printf("info score cp %d depth %d nodes %lld time %d ",bestscore,i,info.nodes,getTime()-info.starttime);
	
	printf("pv");
	for(int j=0;j<numPv;j++)
	{
		printf(" ");
		printMove(PVARRAY[j]);
	}
	printf("\n");
	//printf("Ordering : %0.2f\n",info.fhf/info.fh);
	}
	printf("bestmove ");printMove(bestmove);
	printf("\n");
}

int BOARD::alphabeta(int depth,int alpha,int beta,bool donull)
{
	if(depth==0)
	{ 
		//info.nodes++;
		return quiescence(alpha,beta);
		//return evaluate();
	}

	
	if((info.nodes & 2047) == 0)
	check();
	info.nodes++;
	

	
	if((repeat() || fifty >=100) & ply)
	return 0;

	if(ply >MAXDEPTH -1)
	return evaluate();
	
		bool kingsafe;
		int pos=(int)log2(board[side+4] & -board[side+4]);
		kingsafe=isSafe(pos,side);
		if(!kingsafe)
		depth++;
	
	int score = -INFINITE;
	
/*	if(donull && ply && kingsafe && depth >4 && bigPiece[side] >0)
	{
		makeNullMove();
		score=-alphabeta(depth-4,-beta,-beta+1,false);
		unmakeNullMove();
		if(info.stopped)
		return 0;
		
		if(score>=beta)
		return beta;
	}*/
	
	int legal=0;
	int oldalpha=alpha;
	int bestmove=NOMOVE;
	score=-INFINITE;
	MOVE_LIST moveList;
	moveList.count=0;
	generateAllMoves(moveList);
	

	int pvMove=getPvMove();
	if(pvMove != NOMOVE)
	{
		for(int i=0;i<moveList.count;i++)
		{
			if(moveList.l[i].move == pvMove)
			{
				moveList.l[i].score=2000000;
				break;			
			}
		}
	}

	std::sort(moveList.l,moveList.l+moveList.count,so);

	for(int i=0;i<moveList.count;i++)
	{
		//pickBest(i,moveList);
		if(!makeMove(moveList.l[i].move))
		continue;
		legal++;
		score=-alphabeta(depth-1,-beta,-alpha,donull);
		unmakeMove();

		if(info.stopped)
		return 0;
		
		if(score >alpha)
		{
			if(score >=beta)
			{
				if(legal==1)
				info.fhf++;
				info.fh++;
			if(CAP(moveList.l[i].move) == EMPTY)
			{
				searchKillers[1][depth]=searchKillers[0][depth];
				searchKillers[0][depth]=moveList.l[i].move;				
			}
			return beta;	//beta - cutoff
			}
			alpha=score;
			bestmove=moveList.l[i].move;
			if(CAP(moveList.l[i].move) == EMPTY)
			searchHistory[cboard[FROM(bestmove)]][TO(bestmove)]+=depth;
		}		
	}

	if(legal == 0)
	{
		if(!kingsafe)
		return -MATE + ply;
		else
		return 0;
	}

	if(alpha != oldalpha)
	storePv(bestmove);
	
	return alpha;
}
int BOARD::quiescence(int alpha,int beta)
{
	if((info.nodes & 2047) ==0)
	check();

	info.nodes++;
	
	if(repeat() || fifty >=100)
	return 0;

	if(ply >MAXDEPTH -1)
	return evaluate();

	int value=evaluate();
	if(value >= beta)
	return beta;
	
	if(value >alpha)
	alpha=value;
	
	int legal=0;
	int oldalpha=alpha;
	int bestmove=NOMOVE;
	int score=-INFINITE;

	MOVE_LIST moveList;
	moveList.count=0;
	generateAllCap(moveList);
	
	
	std::sort(moveList.l,moveList.l+moveList.count,so);

	for(int i=0;i<moveList.count;i++)
	{
		//pickBest(i,moveList);
		if(!makeMove(moveList.l[i].move))
		continue;
		score=-quiescence(-beta,-alpha);
		unmakeMove();

		if(info.stopped)
		return 0;

		legal++;
		if(score >alpha)
		{
			if(score >=beta)
			{
				if(legal==1)
				info.fhf++;
				info.fh++;
			return beta;	//beta - cutoff
			}
			alpha=score;
			bestmove=moveList.l[i].move;
		}		
	}
	if(alpha != oldalpha)
	storePv(bestmove);
	
	return alpha;	
}



