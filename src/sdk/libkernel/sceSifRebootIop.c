// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
extern char D_002AACD0[];
extern char D_002AACE0[];
int func_00236960(const char *, ...);
void func_00236A60(int);
void func_00236C00(void);
int func_00239198(const char *, int);
int sceSifRebootIop(const char *img)
{
    char param[80];
    char *s = D_002AACD0;
    register char *d asm("$3");
    register int check asm("$2");
    int byte;
    const char *p = img;
    unsigned int origin;
    if (*p) {
        origin = (unsigned int)img - 11;
        do { ++p; } while (*p);
    } else {
        origin = (unsigned int)img - 11;
    }
    if ((unsigned int)p - origin > 80) {
        func_00236960(D_002AACE0, img);
        return 0;
    }
    func_00236A60(0);
    func_00236C00();
    {
        register char *q asm("$3") = param;
        while ((check = *s) != 0)
            *q++ = *s++;
        d = q;
    }
    byte = (unsigned char)*img;
    if (byte) do {
        *d++ = byte;
        check = *++img;
        byte = check;
    } while (check);
    *d = 0;
    return func_00239198(param, 0);
}
