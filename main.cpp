#include<bits/stdc++.h>
#include<unistd.h>
#include<cstdio>
#include<iostream>
#include<cmath>
#include<cstring>
#include<cstdlib>
#include<windows.h>
#include"global.h"
#include"globalinit.cpp"
#include"hashlists.cpp"
#include"board.cpp"
#include"moves.cpp"
#include"init.cpp"
#include"movegen.cpp"
#include"make.cpp"
#include"eval.cpp"
#include"search.cpp"
#include"perft.cpp"
#include"uci.cpp"

#define FEN "2bqkbnr/1p1ppppp/r7/p2P4/3Q4/4B3/PPP2PPP/RN2KBNR w KQk - 1 7 " 

using namespace std;

int main()
{
globalInit();
uciLoop();
	
return 0;
}

