// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
__asm__(".macro sq r, m\n\tsd \\r, \\m\n\t.endm\n\t.macro lq r, m\n\tld \\r, \\m\n\t.endm");
// (Claude agent) near miss: 17/34, target keeps %hi(sema) in s1 across the loop (frame 0x30), ours 0x20
struct iob { int x0, x4, x8, xC; };
extern int D_002C4A88;
extern struct iob D_0044BB80[];
void _sceFsIobSemaMK(void);
int func_00234A60(int);
int func_00234A40(int);
struct iob *new_iob(void)
{
    struct iob *io;
    _sceFsIobSemaMK();
    func_00234A60(D_002C4A88);
    for (io = D_0044BB80; io < D_0044BB80 + 32; io++) {
        if (io->x4 == 0) {
            io->x4 = 0x10000000;
            func_00234A40(D_002C4A88);
            return io;
        }
    }
    func_00234A40(D_002C4A88);
    return 0;
}
