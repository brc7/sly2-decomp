// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
extern int gStreamingInited;
extern int gSSReadDone;
extern int gStats[];
int func_00241E88(void) __asm__("sceCdSync");
void InvalidDCache(void *, void *);
void snd_FlushSoundCommands(void);

int snd_StreamSafeCdSync(int mode)
{
    char *pe;
    if (!gStreamingInited)
        return func_00241E88();
    pe = (char *)gStats + 0x3F;
    InvalidDCache(gStats, pe);
    gSSReadDone = (gStats[0] == 0);
    if (gSSReadDone == 1)
        return 0;
    if (mode == 1)
        return 1;
    while (!gSSReadDone) {
        snd_FlushSoundCommands();
        InvalidDCache(gStats, pe);
        gSSReadDone = (gStats[0] == 0);
    }
    return 0;
}
