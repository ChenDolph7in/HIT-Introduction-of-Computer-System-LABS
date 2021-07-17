#include <stdio.h>
#include <stdlib.h>

unsigned long Fibon2(int n)//循环
{
	unsigned long a1 = 1,a2 = 1,sum = 0;
	int i = 0;
	if (n == 0)
		return 0;
	else if (n == 1 ||n == 2 )
        return 1;
	else{
		for (i = 0; i<n-2; i++){
			sum = a1 + a2;
			a1 = a2;
			a2 = sum;
		}
		return sum;
	}
}
int main()
{
    for (int i = 0; i < 60; i++)
        printf("fib(%d)=%lu\n", i,Fibon2(i));
	return 0;
}
