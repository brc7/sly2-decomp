// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
__asm__(".macro sq r, m\n\tsd \\r, \\m\n\t.endm\n\t.macro lq r, m\n\tld \\r, \\m\n\t.endm");
// (Claude agent) near miss: 3/26, prologue order only (target sd s0 before sd ra; Sony saves low regs first)
int func_00234AD0(unsigned int *);
int func_00234AC0(unsigned int *);
int PatchIsNeeded(void)
{
    unsigned int a, b;
    func_00234AD0(&a);
    b = (a & 0xFFFF1FFF) | 0x2000;
    func_00234AC0(&b);
    func_00234AD0(&b);
    func_00234AC0(&a);
    return ((b >> 13) & 7) == 0;
}
