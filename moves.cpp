void BOARD::generateMoveset()
{

	bb j,k,l;
/*=======================================================
	Generating all possible moves for ROOK based 
	for all positions on the board    
===========================================================*/
	 j=0xff;
	 k=0x101010101010101;
	 l=k;
	moves[ROOK][0]=j|k;
	for(int i=1;i<64;i++)
	{
		if(i%8==0)
		{
			j=j<<8;
			l=k;
		}
		else
			l=l<<1;
		moves[ROOK][i]=j|l;
	
	}



/*=====================================================
	Generating all possible moves for KING based 
	for all positions on the board    
=======================================================*/
	
	// for 00
	j=0x303;
	moves[KING][0]=j;

	// for 01 to 06
	j=0x707;
	for(int i=1;i<7;i++)
	{
		moves[KING][i]=j;
		j=j<<1;

	}
	// for 07
	moves[KING][7]=0xc0c0;

	// for 57 to 62
	j=0x707000000000000;
	for(int i=57;i<63;i++)
	{
		moves[KING][i]=j;
		j=j<<1;
	}
	//for 08 to 56
	j=0x30303;
	for(int i=8;i<64;i+=8)
	{
		moves[KING][i]=j;
		j=j<<8;
	}
	//for 15 to 63
	j=0xc0c0c0;
	for(int i=15;i<64;i+=8)
	{
		moves[KING][i]=j;
		j=j<<8;
	}

	// for inside square
	j=0x70707;
	for(int i=1;i<=6;i++)
	{
		for(int k=i*8+1;k<(i+1)*8-1;k++)
		{
			moves[KING][k]=j;
			j=j<<1;
		}
			j=j<<2;
	}




/*===========================================================
	Generating all possible moves for BISHOP 
	for all positions on the board    
==============================================================*/
	
	/* GENERATING LOWER RIGHT TRIANGLE FOR FORWARD SLASH*/
	j=0x80;
	moves[BISHOP][7]=j;

	for(int i=6;i>=0;i--)
	{
	j=j<<8;
	moves[BISHOP][i]=(moves[BISHOP][i+1]>>1)|j;
	
	for(int k=1;k<=7-i;k++)
	moves[BISHOP][i+9*k]=moves[BISHOP][i];
	
	}

	/* GENARATING UPPER RIGHT TRIANGLE FOR FORWARD SLASH*/
	
	j=0x100000000000000;
	moves[BISHOP][56]=j;
	for(int i=57;i<=63;i++)
	{
	j=j>>8;
	moves[BISHOP][i]=(moves[BISHOP][i-1]<<1)|j;
	
	for(int k=1;k<=i-56;k++)
	moves[BISHOP][i-9*k]=moves[BISHOP][i];	
	}


	/*GENERATING UPPER RIGHT TRIANGLE FOR BACKWARD SLASH*/
	bb* moves2=(bb*)calloc(64,sizeof(bb));
	j=0x8000000000000000;
	moves2[63]|=j;
	moves[BISHOP][63]|=moves2[63];
	for(int i=62;i>=56;i--)
	{
	j=j>>8;
	moves2[i]=(moves2[i+1]>>1)|j;
	moves[BISHOP][i]|=moves2[i];
	for(int k=1;k<=63-i;k++)
	{
	moves2[i-7*k]=moves2[i];
	moves[BISHOP][i-7*k]|=moves2[i-7*k];
	}	
	}
	free(moves2);
	

	/*GENERATING LOWER LEFT TRIANGLE FOR BACKWARD SALSH*/
	moves2=(bb*)calloc(64,sizeof(bb));
	j=0x1;
	moves2[0]=j;
	moves[BISHOP][0]|=moves2[0];
	for(int i=1;i<8;i++)
	{
	j=j<<8;
	moves2[i]=(moves2[i-1]<<1)|j;
	moves[BISHOP][i]|=moves2[i];
	for(int k=1;k<=i-0;k++)
	{
	moves2[i+7*k]=moves2[i];
	moves[BISHOP][i+7*k]|=moves2[i+7*k];
	}
	}
	

	


/*==============================================================
	Generating all possible moves for QUEEN
	for all possible positions on the board
================================================================*/

	for(int i=0;i<64;i++)
	moves[QUEEN][i]=moves[BISHOP][i]|moves[ROOK][i];


/*===============================================================
	Generating all possible moves for KNIGHT for
	all possible positions on the board
=================================================================*/


	/*From 0 to 7*/
	moves[KNIGHT][0]=0x20400;
	moves[KNIGHT][1]=0x50800;
	moves[KNIGHT][2]=0xa1100;

	for(int i=3;i<=5;i++)
	moves[KNIGHT][i]=moves[KNIGHT][i-1]<<1;

	moves[KNIGHT][6]=0xa01000;
	moves[KNIGHT][7]=0x402000;

	/*From 56 to 63*/
	moves[KNIGHT][56]=0x4020000000000;
	moves[KNIGHT][57]=0x8050000000000;
	moves[KNIGHT][58]=0x110a0000000000;

	for(int i=59;i<=61;i++)
	moves[KNIGHT][i]=moves[KNIGHT][i-1]<<1;

	moves[KNIGHT][62]=0x10a00000000000;
	moves[KNIGHT][63]=0x20400000000000;

	/*From 8 to 56*/
	moves[KNIGHT][8]=0x2040004;
	moves[KNIGHT][16]=0x204000402;
	
	for(int i=24;i<=40;i+=8)
	moves[KNIGHT][i]=moves[KNIGHT][i-8]<<8;


	moves[KNIGHT][48]=0x400040200000000;
	moves[KNIGHT][56]=0x4020000000000;

	/*From 7 to 63*/
	moves[KNIGHT][15]=0x40200020;
	moves[KNIGHT][23]=0x4020002040;
	
	for(int i=31;i<=47;i+=8)
	moves[KNIGHT][i]=moves[KNIGHT][i-8]<<8;


	moves[KNIGHT][55]=0x2000204000000000;
	moves[KNIGHT][63]=0x20400000000000;


	/*From  9 to 13*/
	moves[KNIGHT][9]=0x5080008;
	moves[KNIGHT][10]=0xa110011;
	
	for(int i=11;i<=13;i++)
	moves[KNIGHT][i]=moves[KNIGHT][i-1]<<1;

	moves[KNIGHT][14]=0xa0100010;

	/*From to 49 to 54*/
	moves[KNIGHT][49]=0x800080500000000;
	moves[KNIGHT][50]=0x1100110a00000000;
	
	for(int i=51;i<=53;i++)
	moves[KNIGHT][i]=moves[KNIGHT][i-1]<<1;

	moves[KNIGHT][54]=0x100010a000000000;

	/*From 17 to 41*/
	moves[KNIGHT][17]=0x508000805;
	for(int i=25;i<=41;i+=8)
	moves[KNIGHT][i]=moves[KNIGHT][i-8]<<8;

	/*From 22 to 46*/
	moves[KNIGHT][22]=0xa0100010a0;
	for(int i=30;i<=46;i+=8)
	moves[KNIGHT][i]=moves[KNIGHT][i-8]<<8;

	/*Inner square*/

	moves[KNIGHT][18]=k=0xa1100110a;
	for(int i=0;i<=3;i++)
	{
	moves[KNIGHT][8*(i+2)+2]=k;
	for(int j=8*(i+2)+3;j<=8*(i+3)-3;j++)
	moves[KNIGHT][j]=moves[KNIGHT][j-1]<<1;
	k=k<<8;
	}


/*=====================================================
 Generating all possible moves for White Pawns Movement
=======================================================*/

	k=0x100;
	for(int i=0;i<7;i++)
	{
	moves[WHITE_PAWN_MOVE][i*8]=k;
	for(int j=i*8+1;j<(i+1)*8;j++)
	moves[WHITE_PAWN_MOVE][j]=moves[WHITE_PAWN_MOVE][j-1]<<1;
	k=k<<8;
	}

	


/*=====================================================
 Generating all possible moves for Black Pawns Movement
=======================================================*/
	
	k=0x80000000000000;
	for(int i=8;i>1;i--)
	{
	moves[BLACK_PAWN_MOVE][i*8-1]=k;
	for(int j=i*8-2;j>=(i-1)*8;j--)
	moves[BLACK_PAWN_MOVE][j]=moves[BLACK_PAWN_MOVE][j+1]>>1;
	k=k>>8;
	}


	

/*============================================================
 Generating all possible attack moves for White Pawns Movement
==============================================================*/
	
	k=0x200;
	j=0x4000;
	moves[WHITE_PAWN_ATTACK][0]=k;
	moves[WHITE_PAWN_ATTACK][7]=j;
	
	for(int i=8,j=15;i<56;i+=8,j+=8)
	{
	moves[WHITE_PAWN_ATTACK][i]=moves[WHITE_PAWN_ATTACK][i-8]<<8;
	moves[WHITE_PAWN_ATTACK][j]=moves[WHITE_PAWN_ATTACK][j-8]<<8;
	}

	k=0x500;
	for(int i=0;i<7;i++)
	{
	moves[WHITE_PAWN_ATTACK][i*8+1]=k;
	for(int j=i*8+2;j<(i+1)*8-1;j++)
	moves[WHITE_PAWN_ATTACK][j]=moves[WHITE_PAWN_ATTACK][j-1]<<1;
	k=k<<8;
	}


/*============================================================
 Generating all possible attack moves for Black Pawns Movement
==============================================================*/
	
	k=0x2000000000000;
	j=0x40000000000000;
	
	moves[BLACK_PAWN_ATTACK][56]=k;
	moves[BLACK_PAWN_ATTACK][63]=j;

	for(int i=48,j=55;i>0;i-=8,j-=8)
	{
	moves[BLACK_PAWN_ATTACK][i]=moves[BLACK_PAWN_ATTACK][i+8]>>8;
	moves[BLACK_PAWN_ATTACK][j]=moves[BLACK_PAWN_ATTACK][j+8]>>8;
	}
	k=0x5000000000000;
	
	for(int i=7;i>0;i--)
	{
	moves[BLACK_PAWN_ATTACK][i*8+1]=k;
	for(int j=i*8+2;j<(i+1)*8-1;j++)
	moves[BLACK_PAWN_ATTACK][j]=moves[BLACK_PAWN_ATTACK][j-1]<<1;
	k>>8;
	}
	
}
/*================= ALL MOVES GENARATED==================================*/

/*======================================================================
  Evaluation function for the chess game
========================================================================*/


