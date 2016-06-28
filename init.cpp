
void BOARD::resetMasks()
{
mask[FULL_BOARD]=0xffffffffffffffff;
mask[EMPTY_BOARD]=0x0;
mask[WHITE_OCCUPIED]=board[WHITE_PAWN]|board[WHITE_KING]|board[WHITE_QUEEN]|board[WHITE_BISHOP]|board[WHITE_KNIGHT]|board[WHITE_ROOK];
mask[WHITE_UNOCCUPIED]=~mask[WHITE_OCCUPIED];
mask[BLACK_OCCUPIED]=board[BLACK_PAWN]|board[BLACK_KING]|board[BLACK_QUEEN]|board[BLACK_BISHOP]|board[BLACK_KNIGHT]|board[BLACK_ROOK];
mask[BLACK_UNOCCUPIED]=~mask[BLACK_OCCUPIED];
mask[OCCUPIED]=mask[WHITE_OCCUPIED]|mask[BLACK_OCCUPIED];
mask[UNOCCUPIED]=~mask[OCCUPIED];
}
/*==================================================
Initialize all the bitboards necessary for the game
====================================================*/
void BOARD::initbBoard()
{
board[BLACK_PAWN]=0xff000000000000;
board[BLACK_KING]=0x800000000000000;
board[BLACK_QUEEN]=0x1000000000000000;
board[BLACK_BISHOP]=0x2400000000000000;
board[BLACK_KNIGHT]=0x4200000000000000;
board[BLACK_ROOK]=0x8100000000000000;
board[WHITE_PAWN]=0xff00;
board[WHITE_KING]=0x10;
board[WHITE_QUEEN]=0x8;
board[WHITE_BISHOP]=0x24;
board[WHITE_KNIGHT]=0x42;
board[WHITE_ROOK]=0x81;
resetMasks();
}


/*===============================================
Initialize the global cboard array 
It is the old array representation of the board
=================================================*/
void BOARD::initCboard()
{
	
	memset(cboard,EMPTY,64*sizeof(cb));
	for(int i=8;i<16;i++)
	cboard[i]=WHITE_PAWN;

	for(int i=48;i<56;i++)
	cboard[i]=BLACK_PAWN;

	cboard[0]=WHITE_ROOK;
	cboard[7]=WHITE_ROOK;

	cboard[56]=BLACK_ROOK;
	cboard[63]=BLACK_ROOK;

	cboard[4]=WHITE_KING;
	cboard[3]=WHITE_QUEEN;

	cboard[59]=BLACK_KING;
	cboard[60]=BLACK_QUEEN;

	cboard[1]=WHITE_KNIGHT;
	cboard[6]=WHITE_KNIGHT;

	cboard[57]=BLACK_KNIGHT;
	cboard[62]=BLACK_KNIGHT;

	cboard[2]=WHITE_BISHOP;
	cboard[5]=WHITE_BISHOP;

	cboard[58]=BLACK_BISHOP;
	cboard[61]=BLACK_BISHOP;
}

/*===============================================
Initialize the entire board with all properties
=================================================*/
void BOARD::initialize()
{

	initbBoard();
	initCboard();

	castle=15;
	generateMoveset();
	ply=fifty=full=poskey=side=hisply=0;
	enps=DEFAULTENPS;
	tab.table=(PVENTRY*)calloc(1,sizeof(PVENTRY));

	initPvTable();
}

/*=============================================
===============================================*/
void BOARD::reset()
{
	
	ply=fifty=full=poskey=side=castle=hisply=0;
	enps=DEFAULTENPS;

	for(int i=0;i<TOTAL_BITBOARD;i++)
	board[i]=0;
	
	for(int i=0;i<TOTAL_MASK;i++)
	mask[i]=0;
	
	for(int i=0;i<3;i++)
	{
		bigPiece[i]=0;
		majorPiece[i]=0;
		minorPiece[i]=0;
		material[i]=0;
	}

	memset(cboard,EMPTY,64*sizeof(cb));

	for(int i=0;i<3;i++)
	{
		bigPiece[i]=0;
		majorPiece[i]=0;
		minorPiece[i]=0;
		material[i]=0;
	}

	for(int i=2;i<TOTAL_MASK;i++)
	mask[i]=0;



}

/*=============================================
===============================================*/

void BOARD::parseFen(char* s)
{
	reset();

	int i=0;
	int pos=56;
	int count=0;
	
	while(s[i] != ' ')
	{
		switch(s[i])
		{
		case 'r':{
				board[BLACK_ROOK]|=power[pos];
				cboard[pos]=BLACK_ROOK;
				pos++;
				break;			
			}
		case 'n':{
				board[BLACK_KNIGHT]|=power[pos];
				cboard[pos]=BLACK_KNIGHT;
				pos++;
				break;	
			}
		case 'b':{	
				board[BLACK_BISHOP]|=power[pos];
				cboard[pos]=BLACK_BISHOP;
				pos++;
				break;	
			}
		case 'q':{
				board[BLACK_QUEEN]|=power[pos];
				cboard[pos]=BLACK_QUEEN;
				pos++;
				break;	
			}
		case 'k':{	
				board[BLACK_KING]|=power[pos];
				cboard[pos]=BLACK_KING;
				pos++;
				break;	
			}
		case 'p':{	
				board[BLACK_PAWN]|=power[pos];
				cboard[pos]=BLACK_PAWN;
				pos++;
				break;	
			}
		case 'R':{
				board[WHITE_ROOK]|=power[pos];
				cboard[pos]=WHITE_ROOK;
				pos++;
				break;	
			}
		case 'N':{	
				board[WHITE_KNIGHT]|=power[pos];
				cboard[pos]=WHITE_KNIGHT;
				pos++;
				break;	
			}
		case 'B':{
				board[WHITE_BISHOP]|=power[pos];
				cboard[pos]=WHITE_BISHOP;
				pos++;
				break;	
			}
		case 'Q':{	
				board[WHITE_QUEEN]|=power[pos];
				cboard[pos]=WHITE_QUEEN;
				pos++;
				break;	
			}
		case 'K':{
				board[WHITE_KING]|=power[pos];
				cboard[pos]=WHITE_KING;
				pos++;
				break;	
			}
		case 'P':{
				board[WHITE_PAWN]|=power[pos];
				cboard[pos]=WHITE_PAWN;
				pos++;
				break;	
			}
		case '/':{
				count++;
				pos=(7-count)*8;
				break;
			}
		default:{
				pos+=s[i]-'0';
				break;
			}
		}
	i++;
	
	}

	i++;

	
	resetMasks();

	if(s[i]=='w')
	side=WHITE;
	else
	side=BLACK;

	i+=2;
	
	if(s[i]=='-')
	i+=2;
	else
	{
	while(s[i] !=' ')
	{
		switch(s[i])
		{
			case 'K':{	castle|=WKC;
					break;
				}
			case 'Q':{	castle|=WQC;
					break;
				}
			case 'k':{	castle|=BKC;
					break;
				}
			case 'q':{	castle|=BQC;
					break;
				}
		}
		i++;
	}
	}
		
		i++;

		if(s[i]=='-')
		i+=2;
		else
		{	
			enps=(s[i]-'a')+(s[i+1]-'0'-1)*8;
			i+=3;
		}
		
		
		while(s[i] >='0' && s[i] <='9')
		fifty=fifty*10+s[i++]-'0';
		i++;
		while(s[i] >='0' && s[i] <='9')
		full=full*10+s[i++]-'0';

		generatePoskey();
		updateLists();
	
}


