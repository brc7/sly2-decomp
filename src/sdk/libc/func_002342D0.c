// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
int func_002345B8(int);

int func_002342D0(const char *a, const char *b)
{
    while (*a && func_002345B8(*a) == func_002345B8(*b)) {
        a++;
        b++;
    }
    return func_002345B8(*(unsigned char *)a) - func_002345B8(*(unsigned char *)b);
}
