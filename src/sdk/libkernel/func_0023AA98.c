// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { int x0, x4, x8, xc; int idx; int val; } A;
typedef struct { int x[7]; int *tbl; } B;
void func_0023AA98(A *a, B *b)
{
    b->tbl[a->idx] = a->val;
}
