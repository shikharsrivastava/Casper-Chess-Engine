void BOARD::clearPiece(int pos)
{
	ASSERT(cboard[pos] != EMPTY)

	int piece=cboard[pos];
	//hashing piece
	poskey^=pieceHash[cboard[pos]][pos];
	//clearing bitboard
	board[piece]^=power[pos];
	//Correcting the masks
	resetMasks();
	//Removing piece from cboard
	cboard[pos]=EMPTY;
	// Updating piece lists
	if(isBig[piece])
	bigPiece[color[piece]]--;
	if(isMajor[piece])
	majorPiece[color[piece]]--;
	if(isMinor[piece])
	minorPiece[color[piece]]--;
	//Updating material value
	material[color[piece]]-=materialValue[piece];
}

void BOARD::addPiece(int pos,int piece)
{
	
	ASSERT(cboard[pos] == EMPTY)
	ASSERT(piece != EMPTY)

	//hashing piece
	poskey^=pieceHash[cboard[pos]][pos];
	//Updating bitboard
	board[piece]^=power[pos];
	//Correcting the masks
	resetMasks();
	//Adding piece to cboard
	cboard[pos]=piece;
	// Updating piece lists
	if(isBig[piece])
	bigPiece[color[piece]]++;
	if(isMajor[piece])
	majorPiece[color[piece]]++;
	if(isMinor[piece])
	minorPiece[color[piece]]++;
	//Updating material value
	material[color[piece]]+=materialValue[piece];
}

void BOARD::movePiece(int from,int to)
{

	ASSERT(cboard[from] != EMPTY)
	ASSERT(cboard[to] == EMPTY)

	//Updating Bitboards
	board[cboard[from]]^=power[from];
	board[cboard[from]]^=power[to];
	//Correcting masks
	resetMasks();
	//Change Hash
	poskey^=pieceHash[cboard[from]][from];
	poskey^=pieceHash[cboard[from]][to];
	//Moving piece on board
	cboard[from]=cboard[from]+cboard[to];
	cboard[to]=cboard[from]-cboard[to];
	cboard[from]=cboard[from]-cboard[to];

}

bool BOARD::makeMove(int move)
{
	//Storing in history
	his[hisply].move=move;
	his[hisply].enps=enps;
	his[hisply].castle=castle;
	his[hisply].fifty=fifty;
	his[hisply].poskey=poskey;

	hisply++;
	ply++;
	
	int from=FROM(move);
	int to=TO(move);
	int piece=cboard[from];

	ASSERT(ISVALID(from))
	ASSERT(ISVALID(to))
	ASSERT(piece != EMPTY)
	//Removing castle , enps and side from hash because all this will change
	poskey^=(pieceHash[EMPTY][enps]^castleHash[castle]^sideKey[side]);

	//Checking Special Moves
	//Checking EnPs
	if(EP(move))
	{
		if(color[piece]==WHITE)
		clearPiece(to-8);
		else
		clearPiece(to+8);
	}
	//Checking Castle
	if(CAST(move))
	{
			switch(to)
		{
			case 2:movePiece(0,3);
				break;
			case 6:movePiece(7,5);
				break;
			case 58:movePiece(56,59);
				break;
			case 62:movePiece(63,61);
				break;
			default:printf("Fucked\n");
				exit(1);
		}		
	}

	//In any case now enps is set to default
	enps=DEFAULTENPS;
	
	//Updating castle permissions
	castle&=castlePerm[from];
	castle&=castlePerm[to];
	//Hashing Back Castle
	poskey^=castleHash[castle];
	
	fifty++;
	if(CAP(move) != EMPTY)
	{
		clearPiece(to);
		fifty=0; //fifty again set to zero because captured
	}

	//Checking for new EnPs square and fifty move to zero
	if(TYPE(piece) == PAWN)
	{
		fifty=0;//Setting fifty to zero
		if(PS(move))
		{
			if(color[piece] == WHITE)
			enps=to-8;
			else
			enps=to+8;
		}
	}
	//Now hash in new EnPs
	poskey^=pieceHash[EMPTY][enps];	
	
	//Now we can move our original piece
	movePiece(from,to);

	if(PROM(move) != EMPTY)
	{
		clearPiece(to);
		addPiece(to,PROM(move));	
	}

	
	int kingpos;
	if(side==WHITE)
	{
		kingpos=(int)log2(board[WHITE_KING] & -board[WHITE_KING]);
		side=BLACK;
	}
	else
	{
		kingpos=(int)log2(board[BLACK_KING] & -board[BLACK_KING]);
		side=WHITE;
	}
	//Hashing back new side
	poskey^=sideKey[side];
	
	if(isSafe(kingpos,(side^1)))
		return true;
	else
	{
		unmakeMove();
		return false;
	}
}

void BOARD::unmakeMove()
{
	hisply--;
	ply--;
	int move=his[hisply].move;
	int from=FROM(move);
	int to=TO(move);
	int prevside=side^1;
	
	ASSERT(ISVALID(from))
	ASSERT(ISVALID(to))
	
	//hashing out all flags
	poskey^=(pieceHash[EMPTY][enps]^castleHash[castle]^sideKey[side]);
	
	
	enps=his[hisply].enps;
	fifty=his[hisply].fifty;
	castle=his[hisply].castle;
	
	//hashing in some flags
	poskey^=(pieceHash[EMPTY][enps]^castleHash[castle]);

	if(EP(move))
	{
		if(prevside==WHITE)
		addPiece(to-8,BLACK_PAWN);
		else
		addPiece(to+8,WHITE_PAWN);
	}
	
	if(CAST(move))
	{
			switch(to)
		{
			case 2:movePiece(3,0);
				break;
			case 6:movePiece(5,7);
				break;
			case 58:movePiece(59,56);
				break;
			case 62:movePiece(61,63);
				break;
			default:printf("Fucked\n");
				exit(1);
		}		
	}
	
	movePiece(to,from);
	
	if(CAP(move) != EMPTY)
		addPiece(to,CAP(move));
	
	if(PROM(move) != EMPTY)
	{
		clearPiece(from);
		if(prevside==WHITE)
		addPiece(from,WHITE_PAWN);
		else
		addPiece(from,BLACK_PAWN);
	}
	//poskey=his[hisply].poskey;
	side=prevside;
	poskey=his[hisply].poskey;
	return;
}

int BOARD::parseMove(char *m)
{
	int move;
	if(m[0]=='u')
	{
		unmakeMove();
		return -2;
	}
	else
	if(m[0]=='q')
	return -1;
	else if(m[0] == 's')
	return -3;

	MOVE_LIST moveList;
	moveList.count=0;
	generateAllMoves(moveList);
	

	if(m[1]>'8' || m[1]<'1')return NOMOVE;
	if(m[3]>'8' || m[3]<'1')return NOMOVE;
	if(m[0]>'h' || m[0]<'a')return NOMOVE;
	if(m[0]>'h' || m[0]<'a')return NOMOVE;

	int from=(m[0]-'a')+(m[1]-'1')*8;
	int to=(m[2]-'a')+(m[3]-'1')*8;

	for(int i=0;i<moveList.count;i++)
	{
		move = moveList.l[i].move;
		if(FROM(move) == from && TO(move) == to)
		{
			if(PROM(move) != EMPTY)
			{

				if(m[4] == 'b' && TYPE(PROM(move)) == BISHOP)
				return move;
				else if(m[4] == 'r' && TYPE(PROM(move)) == ROOK)
				return move;
				else if(m[4] == 'k' && TYPE(PROM(move)) == KNIGHT)
				return move;
				else if(m[4] == 'q' && TYPE(PROM(move)) == QUEEN)
				return move;
				else 
				return 0;
			}
			else
			return move;
		}
	}
	return 0;
}

void BOARD::makeNullMove()
{
	his[hisply].move=NOMOVE;
	his[hisply].enps=enps;
	his[hisply].castle=castle;
	his[hisply].fifty=fifty;
	his[hisply].poskey=poskey;

	hisply++;
	ply++;
	poskey^=(pieceHash[EMPTY][enps]^sideKey[side]);
	side=side^1;
	enps=DEFAULTENPS;
	poskey^=(pieceHash[EMPTY][enps]^sideKey[side]);
	
}

void BOARD::unmakeNullMove()
{
	hisply--;
	ply--;
	
	poskey^=(pieceHash[EMPTY][enps]^sideKey[side]);
	enps=his[hisply].enps;
	side=side^1;
	poskey^=(pieceHash[EMPTY][enps]^sideKey[side]);
}
