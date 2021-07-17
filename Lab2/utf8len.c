#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int utf8len(char *p);
int main(){
    char str[100];
    printf("input a string:\n");
    //scanf("%s\n",str);
    fgets(str,100,stdin);
  	str[strlen(str)-1]='\0';
    int len = utf8len(str);
    printf("length:%d\n",len);
    return 0;
}
int utf8len(char *p){
    int len=0;
    for(int i=0,len1=0;p[i]!='\0';i+=len1){
        unsigned char word = p[i];
        if(word >=0xFC)
            len1=6;
        else if(word>=0xF8)
            len1=5;
        else if(word>=0xf0)
            len1=4;
        else if(word>=0xE0)
            len1=3;
        else if(word>=0xC0)
            len1=2;
        else
            len1=1;
        len++;
    }
    return len;
}
