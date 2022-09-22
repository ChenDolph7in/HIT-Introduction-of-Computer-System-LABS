void psum_Loopunrolling(float a[], float p[], long n){
    long i;
    /*last_val holds p[i-1]; val holds p[i] */
    float last_val, val1, val2, val3;
    last_val = p[0] = a[0];
    for(i = 1;i < n-2; i+=3){
        val1 = last_val + a[i];
        val2 = last_val + (a[i] + a[i+1]);
        val3 = last_val + (a[i] + a[i+1] + a[i+2]);

        p[i] = val1;
        p[i+1] = val2;
        p[i+2] = val3;

        last_val = val1 + (a[i] + a[i+1] + a[i+2]);
    }
    for(; i<n; i++){
        val1 = last_val + a[i];
        p[i] = val1;
        last_val = val1;
    }
}
