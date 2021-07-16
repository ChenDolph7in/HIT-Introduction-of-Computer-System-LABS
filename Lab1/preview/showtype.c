#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *pf=NULL;
    int filelength=0;
    int i=0;
    char *buf;
    pf=fopen("D:/hitics/hellolinux.c","r");
    if(pf==NULL)
    {
        return 0;
    }
    else
    {
        fseek(pf,0,SEEK_END);
        filelength=ftell(pf);
        rewind(pf);
        buf=malloc(filelength+1);
        buf[filelength]=0;

        fread(buf,filelength,1,pf);
        fclose(pf);

        for(i=0;i<filelength;i+=16){
            int j;
            if(i>=filelength)
                break;
            for(j=0;j<16;j++){
                if(buf[i+j]=='\n')
                    printf("\\n");
                else if(buf[i+j]==' ')
                    printf("\\0");
                else
                    printf("%c ",buf[i+j]);

                if(i+j>=filelength)
                    break;
            }
            printf("\n");
            for(j=0;j<16;j++){
                printf("%02x",buf[i+j]);
                if(i+j>=filelength)
                    break;
            }
            printf("\n");
        }
        free(buf);
    }
    return 1;
}
