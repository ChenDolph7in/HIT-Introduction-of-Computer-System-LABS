#include <stdio.h>
#include <stdlib.h>

int main()
{
    int ii=0;
    char str[100];
    scanf("%d", &ii);
    int len = 0,a = ii;
    while(a){//获得整形变量的数字位数
        a/=10;
        len++;
    }
    for (int i=len-1; i >= 0; i--){//使用模10从高位到低位获取每一位的数字
        int m=ii%10;
        ii/=10;
        str[i]=m+'0';
    }
    str[len]=' ';
    printf("%s", str);
}
