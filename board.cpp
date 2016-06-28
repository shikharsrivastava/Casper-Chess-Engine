
/*=================================================
Prints the bitboard in chess board representation
argument passed - > bb(datatype) board
====================================================*/
void printBitBoard(bb x)
{
int a[64];
for(int i=0;i<64;i++)
{
a[i]=x%2;
x=x/2;
}
for(int j=56;j>=0;j-=8)
{
for(int i=j;i<j+8;i++)
std::cout<<a[i]<<" ";
std::cout<<std::endl;
}
std::cout<<std::endl;
}

/*===============================================
Checks the state of all present bitboards
=================================================*/
void BOARD::checkState()
{
// WHITE
std::cout<<"WHITE_PAWN\n";
printBitBoard(board[WHITE_PAWN]);
std::cout<<"WHITE KING\n";
printBitBoard(board[WHITE_KING]);
std::cout<<"WHITE_QUEEN\n";
printBitBoard(board[WHITE_QUEEN]);
std::cout<<"WHITE_BISHOP\n";
printBitBoard(board[WHITE_BISHOP]);
std::cout<<"WHITE_KNIGHT\n";
printBitBoard(board[WHITE_KNIGHT]);
std::cout<<"WHITE_ROOK\n";
printBitBoard(board[WHITE_ROOK]);

// BLACK

std::cout<<"BLACK_PAWN\n";
printBitBoard(board[BLACK_PAWN]);
std::cout<<"BLACK_KING\n";
printBitBoard(board[BLACK_KING]);
std::cout<<"BLACK_QUEEN\n";
printBitBoard(board[BLACK_QUEEN]);
std::cout<<"BLACK_BISHOP\n";
printBitBoard(board[BLACK_BISHOP]);
std::cout<<"BLACK_KNIGHT\n";
printBitBoard(board[BLACK_KNIGHT]);
std::cout<<"BLACK_ROOK\n";
printBitBoard(board[BLACK_ROOK]);

// MASKS
std::cout<<"FULL_BOARD\n";
printBitBoard(mask[FULL_BOARD]);
std::cout<<"EMPTY_BOARD\n";
printBitBoard(mask[EMPTY_BOARD]);
std::cout<<"WHITE_OCCUPIED\n";
printBitBoard(mask[WHITE_OCCUPIED]);
std::cout<<"WHITE_UNOCCUPIED\n";
printBitBoard(mask[WHITE_UNOCCUPIED]);
std::cout<<"BLACK_OCCUPIED\n";
printBitBoard(mask[BLACK_OCCUPIED]);
std::cout<<"BLACK_UNOCCUPIED\n";
printBitBoard(mask[BLACK_UNOCCUPIED]);
std::cout<<"OCCUPIED\n";
printBitBoard(mask[OCCUPIED]);
std::cout<<"UNOCCUPIED\n";
printBitBoard(mask[UNOCCUPIED]);

// BOARD VALUES
printf("Poskey %llx\n",poskey);
printf("ply = %d\n",ply);
printf("Hisply = %d\n",hisply);
std::cout<<"Side :"<<side<<std::endl;	
std::cout<<"En Peasent : "<<enps<<std::endl;
std::cout<<"Fifty : "<<fifty<<std::endl;
std::cout<<"Full moves :"<<full<<std::endl;
std::cout<<"Castle permissions  :"<<castle<<std::endl;
}

/*===============================================
Print the global cboard in the standard
chess board format in  the console
=================================================*/

void BOARD::printChessBoard()
{
	for(int i=7;i>=0;i--)
	{
		std::cout<<i+1<<"   ";
	for(int j=8*i;j<8*(i+1);j++)
	{
			switch(cboard[j])
		{
			case BLACK_PAWN: std::cout<<"p   ";break;
					
			case WHITE_PAWN: std::cout<<"P   ";break;
					
			case BLACK_ROOK: std::cout<<"r   ";break;
					
			case WHITE_ROOK: std::cout<<"R   ";break;
					
			case BLACK_BISHOP: std::cout<<"b   ";break;
					
			case WHITE_BISHOP: std::cout<<"B   ";break;
					
			case BLACK_KNIGHT: std::cout<<"n   ";break;
					
			case WHITE_KNIGHT: std::cout<<"N   ";break;
					
			case BLACK_QUEEN: std::cout<<"q   ";break;
					
			case WHITE_QUEEN: std::cout<<"Q   ";break;
					
			case BLACK_KING: std::cout<<"k   ";break;
					
			case WHITE_KING: std::cout<<"K   ";break;
					
			default:	 std::cout<<".   ";break;					

		}
	}
	std::cout<<std::endl;
	}
	printf("  \n ");
	for(int i=0;i<8;i++)
	printf("   %c",'a'+i);
	printf("\n\n");
	printf("Poskey %llx\n",poskey);
	std::cout<<"Side :";
	if(side == BLACK)
	std::cout<<"BLACK\n";
	else
	std::cout<<"WHITE\n";

	//printf("ply = %d\n",ply);
	//printf("Hisply = %d\n",hisply);	
	std::cout<<"En Peasent : "<<boardPos[enps]<<std::endl;
	/*std::cout<<"Fifty : "<<fifty<<std::endl;
	std::cout<<"Full moves :"<<full<<std::endl;*/
	if(castle & WKC)
	printf("K");
	else
	printf("-");
	if(castle & WQC)
	printf("Q");
	else
	printf("-");
	if(castle & BKC)
	printf("k");
	else
	printf("-");
	if(castle & BQC)
	printf("q");
	else
	printf("-");
	printf("\n");
	
	
	
}

static void printPiece(int p)
{
		switch(p)
		{
			case BLACK_PAWN: std::cout<<"BLACK_PAWN ";return;	
			
			case WHITE_PAWN: std::cout<<"WHITE_PAWN "; return;
					
			case BLACK_ROOK: std::cout<<"BLACK_ROOK ";return;
					
			case WHITE_ROOK: std::cout<<"WHITE_ROOK ";return;

			case BLACK_BISHOP: std::cout<<"BLACK_BISHOP ";return;
					
			case WHITE_BISHOP: std::cout<<"WHITE_BISHOP ";return;
					
			case BLACK_KNIGHT: std::cout<<"BLACK_KNIGHT ";return;
					
			case WHITE_KNIGHT: std::cout<<"WHITE_KNIGHT ";return;
					
			case BLACK_QUEEN: std::cout<<"BLACK_QUEEN ";return;
				
			case WHITE_QUEEN: std::cout<<"WHITE_QUEEN ";return;
					
			case BLACK_KING: std::cout<<"BLACK_KING ";return;
			
			case WHITE_KING: std::cout<<"WHITE_KING ";return;
		
			default:	 std::cout<<"NONE ";return;					

		}
}


	
