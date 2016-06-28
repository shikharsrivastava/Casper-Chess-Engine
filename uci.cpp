#define BUFF 400*6
void BOARD::readInp()
{
	int bytes;
	char input[256]="",*endc;
	if(isInput())
	{
		info.stopped=true;
		do 
		{
			bytes=read(fileno(stdin),input,256);
		}
		while(bytes<0);
		
		endc=strchr(input,'\n');
		if(endc)
		*endc=0;
		if(strlen(input) >0)
		{
			if(!strncmp(input,"quit",4))
			info.quit=true;
		}
	}
}


bool isInput()
{
	static int init = 0 ,pipe;
	static HANDLE inh;
	DWORD dw;
	
	if(!init)
	{
		init =1;
		inh=GetStdHandle(STD_INPUT_HANDLE);
		pipe=!GetConsoleMode(inh,&dw);
		if(!pipe)
		{
			SetConsoleMode(inh,dw & ~(ENABLE_MOUSE_INPUT|ENABLE_WINDOW_INPUT));
			FlushConsoleInputBuffer(inh);
		}
	}
	
	if(pipe)
	{
		if(!PeekNamedPipe(inh,NULL,0,NULL,&dw,NULL))
		return 1;
		return dw;
	}
	else
	{
		GetNumberOfConsoleInputEvents(inh,&dw);
		return dw<=1 ? 0:dw;
	}
} 

void BOARD::parsePosition(char *input)
{
	input+=9;
	char *idx=input;
	
	if(!strncmp(idx,"startpos",8))
	parseFen(START_FEN);
	else
	{
		idx=strstr(input,"fen");
		if(idx==NULL)
		parseFen(START_FEN);
		else
		{
			idx+=4;
			parseFen(idx);
		}
	}

	idx = strstr(input,"moves");
	int move;
	
	if(idx != NULL)
	{
		idx+=6;
		while(*idx)
		{
			move=parseMove(idx);
			if(move == NOMOVE)
			break;
			makeMove(move);
			ply=0;
			while(*idx && *idx != ' ')
			idx++;
			idx++;
		}
	}
	printChessBoard();
	
}
void BOARD::parseGo(char *input)
{
	int depth = -1 , movestogo = 30 , movetime = -1 , time = -1 , inc = 0;
	
	info.timeset=false;
	char *p;

	if((p = strstr(input,"infinite")))
	{ 	}
	if((p = strstr(input,"binc")) && side == BLACK)
	inc=atoi(p+5);
	if((p = strstr(input,"winc")) && side == WHITE)
	inc=atoi(p+5);
	if((p = strstr(input,"btime")) && side == BLACK)
	time=atoi(p+6);
	if((p = strstr(input,"wtime")) && side == WHITE)
	time=atoi(p+6);
	if((p = strstr(input,"movestogo")))
	movestogo=atoi(p+10);
	if((p = strstr(input,"movetime")))
	movetime=atoi(p+9);
	if((p = strstr(input,"depth")))
	depth=atoi(p+6);

	if(movetime != -1)
	{
		time=movetime;
		movestogo=1;
	}
	info.starttime=getTime();

	if(time != -1)
	{
		info.timeset=true;
		time/=movestogo;
		time-=50;
		info.stoptime=info.starttime+time+inc;
	}

	if(depth == -1)
	info.depth = MAXDEPTH;
	else
	info.depth=depth;

	printf("time: %d start: %d stop: %d depth: %d timeset: %d\n",time,info.starttime,info.stoptime,info.depth,info.timeset);
	iterativeDeepening();

}
void uciLoop()
{
	setbuf(stdin,NULL);
	setbuf(stdout,NULL);

	char input[BUFF];
	printf("id name %s\n",NAME);
	printf("id author CodeExtractor\n");
	printf("uciok\n");
	
	BOARD b;
	b.initialize();
	

	while(true)
	{
		memset(input,0,sizeof(input));
		fflush(stdout);
		if(!fgets(input,BUFF,stdin))
		continue;
		if(input[0] == '\n')
		continue;

		if(!strncmp(input,"isready",7))
		{
			printf("readyok\n");
			continue;
		}
		else if(!strncmp(input,"position",8))
		b.parsePosition(input);
		else if(!strncmp(input,"ucinewgame",10))
		b.parsePosition("position startpos\n");
		else if(!strncmp(input,"go",2))
		b.parseGo(input);
		else if(!strncmp(input,"quit",4))
		b.info.quit=true;
		else if(!strncmp(input,"uci",3))
		{
			printf("id name %s\n",NAME);
			printf("id author CodeExtractor\n");
			printf("uciok\n");
		}
		else if(!strncmp(input,"null",4))
		{
			b.printChessBoard();
			b.makeNullMove();
			b.printChessBoard();
			b.unmakeNullMove();
			b.printChessBoard();
		}
		if(b.info.quit)
		break;
	}
	
}
