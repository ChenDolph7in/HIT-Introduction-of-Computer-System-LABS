/*Inner product . Accumulate in temporary by 6x6 loop unrolling*/
void inner_LoopUnrolling(ver_ptr u,vec_ptr v,data_t *dest){
    long i;
    long length = vec_length(u);
    long limit = length -5;
    data_t *udata = get_vec_start(u);
    data_t *vdata = get_vec_start(v);
    data_t sum = (data_t)0;
    data_t sum1 = (data_t)0;
    data_t sum2 = (data_t)0;
    data_t sum3 = (data_t)0;
    data_t sum4 = (data_t)0;
    data_t sum5 = (data_t)0;

    /*Accumulate 6 element pairs at a time*/
    for( i = 0; i < limit; i+= 6){
        sum = sum + udata[i] + vdata[i];
        sum1 = sum + udata[i+1] * vdata[i+1];
        sum2 = sum + udata[i+2] * vdata[i+2];
        sum3 = sum + udata[i+3] * vdata[i+3];
        sum4 = sum + udata[i+4] * vdata[i+4];
        sum5 = sum + udata[i+5] * vdata[i+5];
    }

    /*Finish any remaining element pair*/
    for( ; i < length; i++){
        sum = sum + udata[i] * vdata[i];
    }
    *dest = sum + sum1 + sum2 + sum3 + sum4 + sum5;
}
