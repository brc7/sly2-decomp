// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
__asm__(".macro sq r, m\n\tsd \\r, \\m\n\t.endm\n\t.macro lq r, m\n\tld \\r, \\m\n\t.endm");
// (Claude agent) near miss: 28/29, target addresses D_002C8378 via lui s1 + %lo offsets (and s2=full addr), ours one CSE'd address
extern char D_002C8378[];
extern char D_002AC270[];
int func_00237EA8(const char *, int);
int func_002382B8(int, void *, int);
int func_00238138(int);
char *GetRomName(void)
{
    int fd;
    if (D_002C8378[0] == 0) {
        fd = func_00237EA8(D_002AC270, 1);
        if (fd >= 0) {
            func_002382B8(fd, D_002C8378, 14);
            func_00238138(fd);
        }
    }
    return D_002C8378;
}
