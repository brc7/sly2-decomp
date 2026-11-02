// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
__asm__(".macro sq r, m\n\tsd \\r, \\m\n\t.endm\n\t.macro lq r, m\n\tld \\r, \\m\n\t.endm");
// (Claude agent) near miss: 2/15, prologue order only
int func_00234D40(int); void func_00234D60(int);
int func_00240AA0(int a) { int r = func_00234D40(a); func_00234D60(a); return r; }
