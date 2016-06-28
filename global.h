
#define NAME "CaSpEr"

/*=============================================
ASSERT
===============================================*/
#define DEBUG
#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n) \
if(!(n)) { \
printf("%s - FAILED ",#n); \
printf("ON %s \n",__DATE__); \
printf("AT %s \n",__TIME__); \
printf("IN FILE %s \n",__FILE__); \
printf("AT LINE %d \n",__LINE__); \
exit(1); }
#endif

/*==========================================
 All headers for moves.cpp
============================================*/

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
enum {LOLALA , PAWN , KING , QUEEN , BISHOP , KNIGHT , ROOK , WHITE_PAWN_MOVE , BLACK_PAWN_MOVE , WHITE_PAWN_ATTACK , BLACK_PAWN_ATTACK , TOTAL};


/*===============================================
All headers for board.cpp
=================================================*/
typedef unsigned long long bb;
typedef int cb;

enum {WHITE , BLACK , BOTH};
enum	{
	EMPTY = 0 , EMPTY2 ,
      	WHITE_PAWN ,BLACK_PAWN , WHITE_KING , BLACK_KING , WHITE_QUEEN , BLACK_QUEEN ,
	WHITE_BISHOP , BLACK_BISHOP,  WHITE_KNIGHT  , BLACK_KNIGHT , WHITE_ROOK , BLACK_ROOK , 
	TOTAL_BITBOARD
	};


enum 	{ 
	FULL_BOARD , EMPTY_BOARD ,
	WHITE_OCCUPIED , WHITE_UNOCCUPIED ,
	BLACK_OCCUPIED , BLACK_UNOCCUPIED ,
	OCCUPIED , UNOCCUPIED , TOTAL_MASK
	};	
	
#define WKC 1
#define WQC 2 
#define BKC 4
#define BQC 8

bool isBig[TOTAL_BITBOARD]={false,false,false,false,true,true,true,true,true,true,true,true,true,true};
bool isMajor[TOTAL_BITBOARD]={false,false,false,false,true,true,true,true,false,false,false,false,true,true};
bool isMinor[TOTAL_BITBOARD]={false,false,true,true,false,false,false,false,true,true,true,true,false,false};
int materialValue[TOTAL_BITBOARD]={0,0,100,100,50000,50000,1000,1000,325,325,325,325,550,550};

static void globalInit();
static void initHash();

/*===========================================================
		Macros for movesgen.cpp
=============================================================*/
#define FROM(a) ((a) & 0x3f)
#define TO(a) (((a)>>7) & 0x3f)
#define CAP(a) (((a)>>14) &0xf)
#define PROM(a) (((a)>>18) &0xf)
#define EP(a) (((a)>>22) & 1)
#define PS(a) (((a)>>23) &1)
#define CAST(a) (((a)>>24) &1)

#define EPF 0x400000
#define PSF 0x800000
#define CAF 0x1000000
#define CREATEMOVE(f,t,c,p,fl) ((f) | ((t)<<7) | ((c)<<14) | ((p)<<18) | (fl))

#define MAXMOVES 256
#define MAXUNDOMOVES 1024

#define ISVALID(position) ((position >=0 && position <64)?1:0)

#define LEVELDIFF(i,j) ((i/8)-(j/8))

#define TYPE(i) ((i)/2)

int color[TOTAL_BITBOARD]={BOTH,BOTH,WHITE,BLACK,WHITE,BLACK,WHITE,BLACK,WHITE,BLACK,WHITE,BLACK,WHITE,BLACK};
int slideIncr[9]={0,8,-8,1,-1,9,7,-9,-7};
int slideLevel[9]={0,-1,1,0,0,-1,-1,1,1};
int knightIncr[9]={0,17,15,-17,-15,10,6,-10,-6};
int knightLevel[9]={0,-2,-2,2,2,-1,-1,1,1};

struct MOVE
{
	int move;
	int score;
	int index;
};

struct MOVE_LIST
{
	MOVE l[MAXMOVES+1];
	int count;
};

bool so(const MOVE a ,const MOVE b)
{
	return a.score > b.score;
}

/*============================================================================================
			Macros for make.cpp
==============================================================================================*/

int castlePerm[64]={13,15,15,15,12,15,15,14, 15,15,15,15,15,15,15,15, 15,15,15,15,15,15,15,15, 15,15,15,15,15,15,15,15,
		    15,15,15,15,15,15,15,15, 15,15,15,15,15,15,15,15, 15,15,15,15,15,15,15,15, 7,15,15,15,3,15,15,11,	};
char boardPos[65][4]={  "a1\0","b1\0","c1\0","d1\0","e1\0","f1\0","g1\0","h1\0",
				        "a2\0","b2\0","c2\0","d2\0","e2\0","f2\0","g2\0","h2\0",
						"a3\0","b3\0","c3\0","d3\0","e3\0","f3\0","g3\0","h3\0",
						"a4\0","b4\0","c4\0","d4\0","e4\0","f4\0","g4\0","h4\0",
						"a5\0","b5\0","c5\0","d5\0","e5\0","f5\0","g5\0","h5\0",
						"a6\0","b6\0","c6\0","d6\0","e6\0","f6\0","g6\0","h6\0",
						"a7\0","b7\0","c7\0","d7\0","e7\0","f7\0","g7\0","h7\0",
						"a8\0","b8\0","c8\0","d8\0","e8\0","f8\0","g8\0","h8\0","no\0"};

/*============================================================================================
			Macros for hashlists.cpp
==============================================================================================*/

#define RANDHASH (((bb)rand()) | (((bb)rand())<<15) | (((bb)rand())<<30) | (((bb)rand())<<45) | ((((bb)rand())&0xf)<<60) ) 
#define DEFAULTENPS (64)
bb pieceHash[TOTAL_BITBOARD][65];
bb sideKey[3];
bb castleHash[16];

/*================================================================================================
			Macros for search.cpp
==================================================================================================*/
#define NOMOVE 0
#define MAXDEPTH 64
#define INFINITE 1000000000
#define MATE 29000


struct PVENTRY
{
	bb poskey;
	int move;
};

struct PVTABLE
{
	PVENTRY *table;
	int entries;
};


struct SEARCHINFO
{
	int starttime;
	int stoptime;
	int depth;
	int depthset;
	bool timeset;
	int movestogo;
	int infinite;
	
	long long int nodes;
	bool quit;
	bool stopped;
	float fhf;
	float fh;
};

const int victimScore[TOTAL_BITBOARD] = {0,0,100,100,600,600,500,500,300,300,200,200,400,400};
static int mvvlva[TOTAL_BITBOARD][TOTAL_BITBOARD];
static void initMvvlva();
static void pickBest(int ,MOVE_LIST&);

/*=================================================================================================
				Uci protocols	
===================================================================================================*/

void uciLoop();
bool isInput();

/*=================================================================================================
				Global Structures
===================================================================================================*/

struct UNDO_MOVE
{
	int move;
	int castle;
	int fifty;
	int enps;
	bb poskey;
};


enum { COMP , USER };

bb power[64];

void print_bboard(bb);
void print_cboard();
static void printPiece(int);

class BOARD
{
	public:
	
	//Bit boards
	bb board[TOTAL_BITBOARD];
	bb mask[TOTAL_MASK];
	bb moves[TOTAL][64];
	cb cboard[64];
	int searchHistory[TOTAL_BITBOARD][64];
	int searchKillers[2][MAXDEPTH];
	PVTABLE tab;
	int PVARRAY[MAXDEPTH];
	SEARCHINFO info;
	// unique board variables
	int ply;
	int hisply;
	int enps;
	int fifty;
	int full;
	bb poskey;
	int side;
	int castle;
	UNDO_MOVE his[MAXUNDOMOVES+1];
	
	int bigPiece[3];
	int majorPiece[3];
	int minorPiece[3];
	int material[3];
	// Itinialisation functions
	void initialize();
	void initCboard();
	void initbBoard();
	void generateMoveset();
	void reset();
	void resetMasks();
	void parseFen(char *s);

	// Cheking functions
	void checkState();
	void printChessBoard();

	// Actions on board
	

	//Hash and lists functions
	void generatePoskey();
	void updateLists();

	//Move generation
	void printMove(MOVE);
	void printMove(int);
	void printMoveList(MOVE_LIST &);
	void quietMove(int,MOVE_LIST&);
	void captureMove(int,MOVE_LIST&);
	void enpsMove(int,MOVE_LIST&);
	void whitePawnQuiet(int,int,MOVE_LIST&);
	void whitePawnCapture(int,int,MOVE_LIST&);
	void whitePawnMove(MOVE_LIST&);
	void blackPawnQuiet(int,int,MOVE_LIST&);
	void blackPawnCapture(int,int,MOVE_LIST&);
	void blackPawnMove(MOVE_LIST&);
	void slideMove(int,MOVE_LIST&);
	void generateKingMove(int,MOVE_LIST&);
	void generateKnightMove(int,MOVE_LIST&);
	void generateAllMoves(MOVE_LIST&);
	void generateAllCap(MOVE_LIST&);
	bool isSafe(int,int);

	//making moves
	void addPiece(int,int);
	void clearPiece(int);
	void movePiece(int,int);
	bool makeMove(int);
	void unmakeMove();
	void makeNullMove();
	void unmakeNullMove();
	int parseMove(char *);
	//perft test
	void perftTest(int);
	int dfs(int);
	
	//searching functions
	bool repeat();
	int getTime();
	void initPvTable();
	void storePv(int move);
	int getPvLine(int depth);	
	int getPvMove();
	void clearPv();
	
	void check();
	void clearForSearch();
	int evaluate();
	void iterativeDeepening();
	int alphabeta(int,int,int,bool);
	int quiescence(int,int);
	
	//uci protocols
	void parsePosition(char *);
	void parseGo(char*);
	void readInp();
};



