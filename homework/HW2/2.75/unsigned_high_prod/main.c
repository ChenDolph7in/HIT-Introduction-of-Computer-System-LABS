#include <stdio.h>
#include <inttypes.h>

int signed_high_prod(int x, int y);
unsigned unsigned_high_prod(unsigned x, unsigned y);
unsigned unsigned_high_prod_real(unsigned x, unsigned y);

int main()
{
    printf("%d\n",unsigned_high_prod_real(4294967295,4294967295)==unsigned_high_prod(4294967295,4294967295));
}
int signed_high_prod(int x, int y)
{
    int64_t result = (int64_t) x * y;
    result = result >>32;
    return result;
}
unsigned unsigned_high_prod(unsigned x, unsigned y)
{
    int x_31 = x >> 31,y_31 = y >> 31;
    int signed_31 = signed_high_prod(x, y);
    return signed_31 + x * y_31 + y * x_31;//+x_31*y_31<<32
}
unsigned unsigned_high_prod_real(unsigned x, unsigned y)
{
    uint64_t result = (uint64_t) x * y;
    result = result >>32;
    return result;
}
