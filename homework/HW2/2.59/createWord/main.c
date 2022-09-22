#include <stdio.h>
#include <stdlib.h>
size_t createWord(size_t num1,size_t num2);
int main()
{
    int x=0x89abcdef,y=0x76543210;
    //scanf("%d %d",&x,&y);
    printf("\nresult=%x\n",createWord(x,y));
    return 0;
}
size_t createWord(size_t num1,size_t num2){
    size_t bytePos=0xff;
    return  ((num1&bytePos)|(num2&~bytePos));
}
