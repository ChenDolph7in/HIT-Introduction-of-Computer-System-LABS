#include <stdio.h>
#include <stdlib.h>

int Fibon1(int n)//递归
{
	if (n == 0)
		return 0;
	else if(n == 1)
        return 1;
	else
		return Fibon1(n - 1) + Fibon1(n - 2);
}
int main()
{
    for (int i = 0; i < 60; i++)
        printf("fib(%d)=%d\n", i,Fibon1(i));
	return 0;
}
