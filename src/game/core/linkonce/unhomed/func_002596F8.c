// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef struct { char pad[0xE4]; char *xE4; } S;
extern char D_002DE750[];
void func_00259608(S *, int); int *func_002593B8(S *, int);
void *func_002596F8(S *p, int x)
{
    int *r;
    if (x == 1) { func_00259608(p, 2); r = (int *)(p->xE4 + 0xC0); }
    else r = func_002593B8(p, x);
    if (*r != 2) return D_002DE750;
    return (char *)r + 0x30;
}
