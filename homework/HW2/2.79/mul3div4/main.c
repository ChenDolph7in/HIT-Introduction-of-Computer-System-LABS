#include <stdio.h>
#include <stdlib.h>

int mul3div4(int x){
    x = (x<<1)+x;
    x = x >> 2;
    return x;
}

int main(){
    printf("%d\n",mul3div4(33456));
    printf("%d\n",33456*3/4);
}
