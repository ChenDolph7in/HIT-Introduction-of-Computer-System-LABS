#include <stdio.h>
#include <stdlib.h>
#define BUF_SIZE 8//设置缓冲区大小

void good_echo()
{
    char buf[BUF_SIZE];//创建缓冲区

    printf("Please input:\n");
    while(fgets(buf,BUF_SIZE,stdin)!=NULL){//持续从输入流读取数据
        printf("%s",buf);//持续输出
    }
}

int main()
{
    good_echo();
    return 0;
}
