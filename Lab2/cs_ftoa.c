 #include<stdio.h>
 #include <stdlib.h>
 #define EPS 0.0000000000000001
 void cs_ftoa(double f){
    int integ=(int)f;//得到整数部分
    double dec=f-(int)f;//得到小数部分
    char asis[50],str[50];
    int n=0;
    do{//从整数部分的地位到高位将每一位数字储存到辅助数组中

        asis[n]=integ%10+'0';
        n++;
        integ=integ/10;
    }while(integ);
    int i,j=n-1;
    for(i=0;i<n;i++){//将辅助数组的数字反过来传递给字符串
        str[i]=asis[j--];
    }
    str[n]='.';//在字符串中加入'.'，开始处理小数部分
    j=1;
    double asis_dec=dec;
    double pow=0.1;
    do{//处理小数部分，每次乘10取整数放入数组

        asis_dec*=10;
        str[n+j]=(int)asis_dec+'0';
        j++;
        int sub=(int)asis_dec;
        asis_dec=asis_dec-sub;
        dec-=sub*pow;
        pow*=0.1;
    }while(dec>EPS);//精度限制
    str[n+j]='\0';
    printf("%s",str);
 }
 int main()
 {
    double f;
    printf("Please input a float：\n");
    scanf("%lf", &f);
    cs_ftoa(f);
 }
