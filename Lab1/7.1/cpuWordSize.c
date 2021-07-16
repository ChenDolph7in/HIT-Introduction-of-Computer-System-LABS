#include <stdio.h>
#include <stdlib.h>
int cpuWordSize();
int main()
{
    int *p=NULL;
    if(cpuWordSize()==8)
        printf("64位CPU");
    else
        printf("32位CPU");
    return 0;
}

int cpuWordSize(){
    int *p=NULL;
    return sizeof(p);
}
