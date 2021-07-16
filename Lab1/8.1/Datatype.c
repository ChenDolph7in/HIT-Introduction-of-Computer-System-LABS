#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start,size_t len)
{
    size_t i;
    for(i=0; i<len; i++){
        printf("%.2x ",start[i]);
    }

    printf("\n");
}
void show_int (int x)
{
    show_bytes((byte_pointer) &x, sizeof(int));
}
void show_float(float x)
{
    show_bytes((byte_pointer) &x, sizeof(float));
}
void show_pointer(void *x)
{
    show_bytes((byte_pointer) &x, sizeof(void *));
}
void show_double(double x)
{
    show_bytes((byte_pointer) &x, sizeof(double));
}
void show_char(char x)
{
    show_bytes((byte_pointer) &x, sizeof(char));
}

struct
{
    char *name;
    int id;
    double idnumber;
} s={"Chen Dongxin",1190201308,23118220010422753};

char c= 'a';
int i = 1190201308;
float f =23118220010422753;
double d = 23118220010422753;
int main()
{
    printf("s.name：\t%s\t\t\t,  地址:%d\t,  内存字节：",s.name,&s.name);
    show_bytes(s.name,strlen(s.name));
    printf("s.id：\t\t%d\t\t\t,  地址:%d\t,  内存字节：",s.id,&s.id);
    show_int(s.id);
    printf("s.idnumber:\t%lf\t,  地址:%d\t,  内存字节：",s.idnumber,&s.idnumber);
    show_double(s.idnumber);


    printf("\nc：\t\t%c\t\t\t\t,  地址:%d\t,  内存字节：",c,&c);
    show_char(c);
    printf("i：\t\t%d\t\t\t,  地址:%d\t,  内存字节：",i,&i);
    show_int(i);
    printf("f：\t\t%f\t,  地址:%d\t,  内存字节：",f,&f);
    show_float(f);
    printf("d：\t\t%lf\t,  地址:%d\t,  内存字节：",d,&d);
    show_double(d);
    printf("main地址\t：%p\n",&main);
    printf("printf地址\t：%p\n",&printf);
    return 0;
}


