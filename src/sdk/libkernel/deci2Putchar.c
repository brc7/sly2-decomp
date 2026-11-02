// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
extern int D_002C49EC;
extern char D_00448D50[];
void func_0023AA40(char *);
void deci2Putchar(int c)
{
    if (D_002C49EC >= 0x7E) {
        D_002C49EC = 0;
        D_00448D50[0x7F] = 0;
        func_0023AA40(D_00448D50);
    }
    if (c == '\n') {
        D_00448D50[D_002C49EC] = c;
        D_00448D50[D_002C49EC + 1] = 0;
        D_002C49EC = 0;
        func_0023AA40(D_00448D50);
    } else {
        D_00448D50[D_002C49EC++] = c;
    }
}
