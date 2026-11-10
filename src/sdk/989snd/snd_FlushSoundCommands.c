// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef struct CB { void (*fn)(int, long); int pad; long arg; } CB;
extern int gCommBusy;
extern void (*gCdCallback)(int);
extern int gSSRead, gSSReadDone, gLoadingFromFS, D_002C4680, gCaching, gLoadBusy;
extern int *D_002C4660[2];
extern CB *gSndCommandReturnDefPtr[2];
extern int *D_002C4678[2];
extern void (*gLoadCB)(int, long);
extern long gLoadUserData;
extern CB gLoadReturnDef;
extern int gSyncBuffer[];
extern unsigned int gLoadReturnValue;
int snd_GotReturns__Fv(void);
void func_00234CA0(int) __asm__("FlushCache");
void snd_SendCurrentBatch__Fv(void);
int snd_StreamSafeCdSync(int);

int snd_FlushSoundCommands(void)
{
    int i;
    if (gCommBusy && snd_GotReturns__Fv()) {
        if (gLoadingFromFS) {
            if (gLoadCB)
                gLoadCB(gSyncBuffer[1], gLoadUserData);
            gLoadCB = 0;
            gLoadingFromFS = 0;
        } else {
            int other = D_002C4680 != 1;
            for (i = 0; i < *D_002C4660[other]; i++) {
                if (gSndCommandReturnDefPtr[other][i].fn)
                    gSndCommandReturnDefPtr[other][i].fn(D_002C4678[other][i + 1], gSndCommandReturnDefPtr[other][i].arg);
            }
        }
    }
    if (gLoadBusy) {
        func_00234CA0(0);
        if (gLoadReturnValue != 0xFFFFFFFF) {
            void (*fn)(int, long) = gLoadReturnDef.fn;
            if (fn) {
                long arg = gLoadReturnDef.arg;
                gLoadReturnDef.fn = 0;
                gLoadReturnDef.arg = 0;
                fn(gLoadReturnValue, arg);
            }
            gLoadReturnValue = 0;
            gLoadBusy = 0;
        }
    }
    if (!gCommBusy) {
        if (*D_002C4660[D_002C4680] && !gCaching)
            snd_SendCurrentBatch__Fv();
    }
    if (gSSRead) {
        snd_StreamSafeCdSync(1);
        if (gSSReadDone) {
            gSSRead = 0;
            gSSReadDone = 0;
            if (gCdCallback)
                gCdCallback(1);
        }
    }
    if (gCommBusy || gLoadBusy)
        return 1;
    return 0;
}

// ---- Common symbols defined here (docs/guides/tu-merging.md "Data", address names): g++ 2.95 emits a class or
// template static data member as COMMON (C: a tentative definition), and the linker allocates commons at the end
// of .bss (small ones in .sbss), which is why these addresses are not in TU order. Sizes are the gaps to the next
// name in the ELF; a 16-aligned start with a 16-multiple size is a qword array (commons take the type's alignment).
typedef int qword __attribute__((mode(TI)));
qword c_00443840[4] __asm__("gSyncBuffer");   /* .bss 0x40 */
