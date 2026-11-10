// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
extern unsigned int D_002C4AA8[4];
extern unsigned int D_002C4AA0;
extern unsigned int D_00239558[];
extern unsigned int D_00239520[];
void SetSyscall_002396B0(unsigned int, unsigned int);
unsigned int FindAddress(unsigned int, unsigned int, unsigned int *);
void InitSystemCallTableAddress(void)
{
    unsigned int a, b, ta, tb;
    SetSyscall_002396B0(D_002C4AA8[0], D_002C4AA8[1]);
    SetSyscall_002396B0(D_002C4AA8[2], D_002C4AA8[3]);
    a = FindAddress(0x80000000, 0x80080000, D_00239558);
    b = FindAddress(0x80000000, 0x80080000, D_00239520);
    ta = a - 0x20C;
    tb = b - 0x168;
    while (ta != tb) {
        if (ta < tb) {
            a = FindAddress(a + 4, 0x80080000, D_00239558);
            ta = a - 0x20C;
        } else {
            b = FindAddress(b + 4, 0x80080000, D_00239520);
            tb = b - 0x168;
        }
    }
    D_002C4AA0 = ta;
}
