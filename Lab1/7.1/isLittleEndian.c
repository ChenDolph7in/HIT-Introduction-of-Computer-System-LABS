#include <stdio.h>
#include <stdlib.h>
int isLittleEndian();
int main()
{
    if(isLittleEndian()==1)
        printf("大端法");
    else
        printf("小端法");
    return 0;
}
int isLittleEndian(){
    int i=1;
    unsigned char *pointer = (unsigned char*)&i;
    if(pointer[0]<pointer[3])
        return 1;
    else
        return 0;
}
