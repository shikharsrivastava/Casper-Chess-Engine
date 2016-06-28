void BOARD::perftTest(int depth)
{
	int ans=0,tmp;
	MOVE_LIST moveList;
	moveList.count=0;
	generateAllMoves(moveList);
	int starttime=getTime();
	for(int i=0;i<moveList.count;i++)
	{
		
		if(!makeMove(moveList.l[i].move))
		continue;
		tmp=dfs(depth-1);
		printf("Move %d : %d\n",i+1,tmp);
		ans+=tmp;
		unmakeMove();
	}
	printf("Test Result : %d  Time = %dms\n",ans,getTime()-starttime);	
	
}

int BOARD::dfs(int depth)
{
	if(depth==0)
	return 1;
	
	int ans=0;
	MOVE_LIST moveList;
	moveList.count=0;	

	generateAllMoves(moveList);
	
	for(int i=0;i<moveList.count;i++)
	{
		if(!makeMove(moveList.l[i].move))
		continue;
			
		ans+=dfs(depth-1);
		unmakeMove();
	
	}
	return ans;
}



