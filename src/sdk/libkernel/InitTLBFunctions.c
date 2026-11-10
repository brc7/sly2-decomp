// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { int no; void *fn; } ENT;
extern ENT D_002C5DE8[8];
extern int D_002C5DE0;
extern char D_002C5A68[];
void SetSyscall_0023B280(int, void *);
void *GetEntryAddress_0023B270(int);
void KCopySyscall_0023B228(void *, void *, int);
void func_00234CA0(int);
void InitTLBFunctions(void)
{
    unsigned int i;
    SetSyscall_0023B280(D_002C5DE8[0].no, D_002C5DE8[0].fn);
    KCopySyscall_0023B228((void *)0x80075000, D_002C5A68, 0x330);
    func_00234CA0(0);
    func_00234CA0(2);
    SetSyscall_0023B280(D_002C5DE8[1].no, D_002C5DE8[1].fn);
    SetSyscall_0023B280(D_002C5DE8[2].no, D_002C5DE8[2].fn);
    for (i = 3; i < 8; i++)
        SetSyscall_0023B280(D_002C5DE8[i].no, GetEntryAddress_0023B270(D_002C5DE8[i].no));
    D_002C5DE0 = (int)GetEntryAddress_0023B270(3);
}
