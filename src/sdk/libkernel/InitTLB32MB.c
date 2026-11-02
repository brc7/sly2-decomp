// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { unsigned int PageMask, EntryHi, EntryLo0, EntryLo1; } TLBENT;
typedef struct {
    int nKernel, nDefault, nExtended, nWired;
    TLBENT *pKernel, *pDefault, *pExtended;
} TLBINFO;
extern TLBINFO D_002C6098;
extern char D_002AAD98[], D_002AADD0[], D_002AADE8[], D_002AAE00[];
int func_002369E0(const char *, ...);
void func_002398D0(int);
void func_0023B358(int, unsigned int, unsigned int, unsigned int, unsigned int);

int InitTLB32MB(void)
{
    register int i __asm__("$25"); /* the original keeps the TLB index in $t9 across calls */
    int n;
    int j;
    TLBENT *p;

    func_002369E0(D_002AAD98, D_002C6098.nKernel - 1, D_002C6098.nKernel,
                  D_002C6098.nKernel + D_002C6098.nDefault - 1,
                  D_002C6098.nKernel + D_002C6098.nDefault,
                  D_002C6098.nKernel + D_002C6098.nDefault + D_002C6098.nExtended - 1);
    __asm__ __volatile__("mtc0 $0, $6\n\tsync.p");

    i = 0;
    n = D_002C6098.nKernel;
    if (n > 48) {
        func_002369E0(D_002AADD0);
        func_002398D0(1);
    }
    p = D_002C6098.pKernel;
    for (; i < n; i++, p++)
        func_0023B358(i, p->PageMask, p->EntryHi, p->EntryLo0, p->EntryLo1);

    n = i + D_002C6098.nDefault;
    if (n > 48) {
        func_002369E0(D_002AADE8);
        func_002398D0(1);
    }
    p = D_002C6098.pDefault;
    for (; i < n; i++, p++)
        func_0023B358(i, p->PageMask, p->EntryHi, p->EntryLo0, p->EntryLo1);

    D_002C6098.nWired = i;
    __asm__ __volatile__("mtc0 %0, $6\n\tsync.p" : : "r"(i));

    if (D_002C6098.nExtended > 0) {
        n = i + D_002C6098.nExtended;
        if (n > 48) {
            func_002369E0(D_002AAE00);
            func_002398D0(1);
        }
        p = D_002C6098.pExtended;
        for (; i < n; i++, p++)
            func_0023B358(i, p->PageMask, p->EntryHi, p->EntryLo0, p->EntryLo1);
    }
    for (j = i; j < 48; j++)
        func_0023B358(j, 0, 0xE0000000 + (j << 13), 0, 0);
    return i;
}
