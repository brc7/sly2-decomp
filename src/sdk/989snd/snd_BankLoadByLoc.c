// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
extern int D_002C4648, gLoadBusy, D_002C46B8;
extern char D_002AA250[], D_002AA280[], D_002AA1C8[], D_002AA2A0[];
extern unsigned int gLoadReturnValue[];
extern int D_004484C0[];
extern char D_00448440[];
int snd_StreamSafeCdSync(int);
void printf(const char *, ...);
void SyncDCache(void *, void *);
void snd_FlushSoundCommands(void);
void func_00234CA0(int) __asm__("FlushCache");
int func_00237488(void *) __asm__("sceSifCheckStatRpc");
int func_00237288(void *, int, int, void *, int, void *, int, void *, void *) __asm__("sceSifCallRpc");

int snd_BankLoadByLoc(int a, int b)
{
    D_002C4648 = 0;
    if (gLoadBusy) {
        if (!D_002C46B8)
            printf(D_002AA250);
        return 0;
    }
    if (snd_StreamSafeCdSync(1) == 1) {
        if (!D_002C46B8)
            printf(D_002AA280);
        return 0;
    }
    gLoadReturnValue[0] = 0xFFFFFFFF;
    SyncDCache(gLoadReturnValue, gLoadReturnValue + 1);
    D_004484C0[0] = a;
    D_004484C0[1] = b;
    while (func_00237488(D_00448440)) {
        if (!D_002C46B8)
            printf(D_002AA1C8);
        snd_FlushSoundCommands();
        func_00234CA0(0);
    }
    if (func_00237288(D_00448440, 3, 1, D_004484C0, 8, gLoadReturnValue, 4, 0, 0) < 0) {
        if (!D_002C46B8)
            printf(D_002AA2A0);
        D_002C4648 = 0x106;
        return 0;
    }
    while (gLoadReturnValue[0] == 0xFFFFFFFF)
        func_00234CA0(0);
    return gLoadReturnValue[0];
}

// ---- Common symbols defined here (docs/guides/tu-merging.md "Data", address names): g++ 2.95 emits a class or
// template static data member as COMMON (C: a tentative definition), and the linker allocates commons at the end
// of .bss (small ones in .sbss), which is why these addresses are not in TU order. Sizes are the gaps to the next
// name in the ELF; a 16-aligned start with a 16-multiple size is a qword array (commons take the type's alignment).
typedef int qword __attribute__((mode(TI)));
qword c_004484C0[4] __asm__("D_004484C0");   /* .bss 0x40 */
