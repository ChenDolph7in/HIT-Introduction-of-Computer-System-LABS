#include <stdio.h>
#include <stdlib.h>

int main()
{
    char str[100];
    scanf("%s", str);
    float float_str = 0;
    int len = strlen(str),dot_pos = 0;
    int i = 0;
    while(str[i]!='.'){//确定小数点位置
        dot_pos++;
        i++;
    }
    float bit_count = 1.0;
    for (i = dot_pos - 1; i >= 0; i--){//按位读取字符串中整数数字加到整数部分
        if (str[i]>= 48&&str[i]<= 57){
            float_str += bit_count*(str[i]-48);
            bit_count *= 10;
        }
    }
    bit_count = 0.1;
    for (i = dot_pos + 1; i < len; i++){//按位读取字符串中小数数字加到小数部分
        if (str[i]>= 48 &&str[i]<= 57){
            float_str += bit_count*(str[i]-48);
            bit_count /= 10;
        }
    }
    printf("%f",float_str);

}
