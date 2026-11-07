// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
extern int D_002C464C;
extern int D_002C4658;
extern int D_00448400[];
int func_00241E88(void) __asm__("sceCdSync");
void func_00235078(void *, void *);
void snd_FlushSoundCommands(void);

int snd_StreamSafeCdSync(int mode)
{
    char *pe;
    if (!D_002C464C)
        return func_00241E88();
    pe = (char *)D_00448400 + 0x3F;
    func_00235078(D_00448400, pe);
    D_002C4658 = (D_00448400[0] == 0);
    if (D_002C4658 == 1)
        return 0;
    if (mode == 1)
        return 1;
    while (!D_002C4658) {
        snd_FlushSoundCommands();
        func_00235078(D_00448400, pe);
        D_002C4658 = (D_00448400[0] == 0);
    }
    return 0;
}
