static void initHash()
{
	for(int piece=0;piece<TOTAL_BITBOARD;piece++)
	{
		for(int pos=0;pos<65;pos++)
		pieceHash[piece][pos]=RANDHASH;
	}
	
	for(int i=0;i<3;i++)
	sideKey[i]=RANDHASH;
	
	for(int i=0;i<16;i++)
	castleHash[i]=RANDHASH;
	
}

static void initMvvlva()
{
	for(int victim=0;victim<TOTAL_BITBOARD;victim++)
	{
		for(int attacker=0;attacker<TOTAL_BITBOARD;attacker++)
		mvvlva[victim][attacker]=victimScore[victim] +6 -(victimScore[attacker]/100);
	}

	/*for(int victim=2;victim<TOTAL_BITBOARD;victim++)
	{
		for(int attacker=2;attacker<TOTAL_BITBOARD;attacker++)
		{
		printPiece(victim);printf(" x ");printPiece(attacker); printf(" = %d\n",mvvlva[victim][attacker]);
		}
	}*/
}

static void globalInit()
{
	power[0]=1;
	for(int i=1;i<64;i++)
	power[i]=power[i-1]*2;
	initHash();
	initMvvlva();
}

