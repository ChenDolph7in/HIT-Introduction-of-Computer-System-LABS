#include <stdio.h>
#include <stdlib.h>
int main()
{
    char str[100];
    printf("Please input a integer:\n");
    scanf("%s", str);
    int len = strlen(str),int_str = 0;
    int bit_count=1;
    for (int i=len-1; i >= 0; i--){//按位读取字符串数字加到整型变量中
        if (str[i] !=' '){
            int_str += bit_count* (str[i] - '0');
            bit_count *= 10;
        }
    }
    printf("%d", int_str);
}
