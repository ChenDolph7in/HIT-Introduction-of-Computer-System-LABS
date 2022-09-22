#include <stdio.h>
#include <stdlib.h>
//32位机器最多只能左移或右移31位
int int_size_is_32(){//至少为32位的机器可运行
    int set_msb = 1<<31;
    int beyond_msb = set_msb<<1;
    return set_msb&&!beyond_msb;
}

int int_size_is_16(){//至少为16位的机器可运行
    int set_msb = 1<<1<<4<<5<<1<<4<<1<<1<<4<<5<<1<<4;
    int beyond_msb = set_msb<<1;
    return set_msb&&!beyond_msb;
}

int main()
{
    printf("%d\n",int_size_is_32());
    printf("%d\n",int_size_is_16());
    return 0;
}
