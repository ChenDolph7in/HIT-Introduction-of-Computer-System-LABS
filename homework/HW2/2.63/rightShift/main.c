#include <stdio.h>
#include <stdlib.h>

unsigned srl(unsigned x,int y){
    /*Perform shift arithmetically*/
    unsigned xsra = (int)x>>y;
    unsigned bytePos = (int)-1<<((sizeof(int)<<3)-y);
    return (xsra&~bytePos);
}
int sra(int x,int y){
    /*Perform shift logically*/
    int xsrl = (unsigned)x >>y;
    int flag=0x80000000;
    if(flag&x){
        return xsrl|((int)-1<<((sizeof(int)<<3)-y));
    }
    else{
        return xsrl;
    }
}
int main()
{
    unsigned num1 = 0x81000000;
    int num2 = 0x81000000;
    int y = 3;
    printf("num1=%x\n",num1);
    printf("num2=%x\n",num2);
    printf("num1>>y=%x\n",srl(num1,y));
    printf("num2>>y=%x\n",sra(num2,y));
    return 0;
}
