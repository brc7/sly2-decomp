// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
/* near miss (2 diffs): addiu a2/addiu a1 order before KCopySyscall_00239728 call (scheduler tie). */
typedef struct { int num; void *func; } SysEnt;
extern SysEnt D_002C5268[];
extern char D_002C4AC0[];
int func_00239780(void);
void SetSyscall_00239718(int, void *);
void KCopySyscall_00239728(void *, void *, int);
void func_00234CA0(int);
void *GetEntryAddress_00239770(int);
void InitExecPS2(void)
{
    SysEnt *t;
    unsigned int i;
    if (func_00239780()) {
        t = D_002C5268;
        SetSyscall_00239718(t[0].num, t[0].func);
        KCopySyscall_00239728((void *)0x80074000, D_002C4AC0, 0x7A8);
        func_00234CA0(0);
        func_00234CA0(2);
        SetSyscall_00239718(t[1].num, t[1].func);
        for (i = 2; i < 3; i++)
            SetSyscall_00239718(t[i].num, GetEntryAddress_00239770(t[i].num));
    }
}
