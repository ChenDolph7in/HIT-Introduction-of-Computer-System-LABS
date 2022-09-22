#include <stdio.h>
#include <stdlib.h>

typedef unsigned packed_t;
//原代码不能正确Q抽取负数
int xbyte(packed_t word,int bytenum){
    int leftbyte = sizeof(unsigned)-(bytenum+1);
    int rightbyte = leftbyte + bytenum;
    return (int)word<<(leftbyte<<3)>>(rightbyte<<3);
}

int main()
{
    printf("result=%d\n",xbyte(-1,0));
    return 0;
}
