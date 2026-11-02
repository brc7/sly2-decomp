// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
/* near miss (2 diffs): arg-setup scheduling of the 2nd func_00239910 call (target: ori a0 before jal, addiu a2 in slot). Same on all 2.95 builds; prototypes/types don't change it. */
typedef struct { int num; void *func; } SysEnt;
extern SysEnt D_002C59F0[];
extern char D_002C5288[];
extern char D_002C59C8[];
void func_00239900(int, void *);
void func_00239910(void *, void *, int);
void func_00234CA0(int);
void *func_00239958(int);
void InitAlarm(void)
{
    SysEnt *t;
    unsigned int i;
    if (!(*(volatile int *)0x10001810 & 0x100)) {
        t = D_002C59F0;
        func_00239900(t[0].num, t[0].func);
        func_00239910((void *)0x80076000, D_002C5288, 0x740);
        func_00239910((void *)0x82000, D_002C59C8, 0x28);
        func_00234CA0(0);
        func_00234CA0(2);
        func_00239900(t[1].num, t[1].func);
        for (i = 2; i < 8; i++)
            func_00239900(t[i].num, func_00239958(t[i].num));
    }
}
