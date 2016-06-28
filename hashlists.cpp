
void BOARD::generatePoskey()
{
	poskey=0;
	for(int i=0;i<64;i++)
	{
		if(cboard[i] != EMPTY)
			poskey^=pieceHash[cboard[i]][i];
			
	}

	poskey^=sideKey[side];
	poskey^=pieceHash[EMPTY][enps];
	poskey^=castleHash[castle];
} 

void BOARD::updateLists()
{
	bb p;
	int clr,pos;

	for(int piece=WHITE_PAWN;piece<TOTAL_BITBOARD;piece++)
	{
		p=board[piece];
		clr=color[piece];
		while(p>0)
		{
			if(isBig[piece])
				bigPiece[clr]++;
		
			if(isMajor[piece])
				majorPiece[clr]++;
		
			if(isMinor[piece])
				minorPiece[clr]++;
		
			p-=p&-p;
			material[clr]+=materialValue[piece];	
		}
		
	}
	
}
