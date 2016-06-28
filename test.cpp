#include<bits/stdc++.h>

using namespace std;

struct s
{
	int x;
	int y;
};
bool so(const s a, const s b)
{	
	return a.x>=b.x;
}

int main()
{
	s arr[2];
	arr[0].x=5;
	arr[0].y=1;
	arr[1].x=5;
	arr[1].y=2;
	
	sort(arr,arr+2,so);
	for(int i=0;i<2;i++)
	{
		printf("%d %d\n",arr[i].x,arr[i].y);
	}
}
