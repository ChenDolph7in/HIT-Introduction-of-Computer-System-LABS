#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

typedef unsigned char* byte_pointer;
void show_bytes(byte_pointer start, size_t len) {
	size_t i;
	for (i = 0; i < len; i++)
		printf("%.2x", start[i]);
}
void show_float(float x) {
	show_bytes((byte_pointer)&x, sizeof(float));
}
int main()
{
    float pluszero=+0.0;
    float negtzero=-0.0;
    float min_f=FLT_TRUE_MIN;
    float max_f=FLT_MAX;
    float minformatf=FLT_MIN;
    float infi=INFINITY;
    float nan=NAN;
    printf("+0: %E\t", pluszero);
    show_float(pluszero);
	printf("\n-0: %E\t", negtzero);
	show_float(negtzero);
	printf("\n最小浮点正数：%E\t", min_f);
	show_float(min_f);
	printf("\n最大浮点正数: %E\t", max_f);
	show_float(max_f);
	printf("\n最小正的规格化浮点数:%E\t", minformatf);
	show_float(minformatf);
	printf("\n正无穷大:%fE\t", infi);
	show_float(infi);
	printf("\nNan:%f.64\t", nan);
	show_float(nan);
    return 0;
}
