#include <stdio.h>
#include <stdlib.h>

typedef unsigned float_bits;

float_bits float_half(float_bits f)
{
    unsigned sig = f>>31;

    unsigned exp = (f>>23)&0xff;

    unsigned frac = f&0x7fffff;
    if(exp==0xff)
        return f;
    if(exp==0){
        if((frac&0x3)==0x3){
            frac=(frac>>1)+1;
        }
        else{
             frac=(frac>>1);
        }
    }
    else if(exp==0x1){
        exp=exp-1;
        if((frac&0x3)==0x3){
            frac=(frac>>1)+1;
            frac=frac+(1<<22);
        }
        else{
             frac=(frac>>1);
             frac=frac+(1<<22);
        }
    }
    else
        exp=exp-1;
    return (sig<<31)|(exp<<23)|frac;

}
