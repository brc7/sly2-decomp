// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
extern int gCommBusy, gCommandFillBuffer, gCaching, D_002C46B8;
extern int *gSndCommandBuffePtr[2];
extern unsigned char D_00443880[];
extern unsigned char D_0044388C[];
extern int gSyncBuffer[];
extern char D_00443800[];
extern char D_002AA5A0[], D_002AA1C8[];
void printf(const char *, ...);
void *func_0023320C(void *, void *, int) __asm__("memcpy");
void snd_FlushSoundCommands(void);
void func_00234CA0(int) __asm__("FlushCache");
void snd_PrepareReturnBuffer(void *, int);
int func_00237488(void *) __asm__("sceSifCheckStatRpc");
int func_00237288(void *, int, int, void *, int, void *, int, void *, void *) __asm__("sceSifCallRpc");
int snd_GotReturns__Fv(void);
void snd_SendCurrentBatch__Fv(void);

int snd_SendIOPCommandAndWait__FiiPc(int cmd, int size, unsigned char *buf)
{
    int i;
    int ret;
    if (cmd != 0x68) {
        for (i = 0; i < size; i++)
            D_00443880[i] = buf[i];
    } else {
        if ((unsigned int)(*(int *)(buf + 8) + 0xC) > 0x200) {
            printf(D_002AA5A0);
            while (1)
                ;
        }
        for (i = 0; i < 0xC; i++)
            D_00443880[i] = buf[i];
        func_0023320C(D_0044388C, *(void **)(buf + 0xC), *(int *)(buf + 8));
    }
    while (gCommBusy) {
        snd_FlushSoundCommands();
        func_00234CA0(0);
    }
    snd_PrepareReturnBuffer(gSyncBuffer, 1);
    while (func_00237488(D_00443800)) {
        if (!D_002C46B8)
            printf(D_002AA1C8);
        snd_FlushSoundCommands();
        func_00234CA0(0);
    }
    if (size)
        func_00237288(D_00443800, cmd, 1, D_00443880, size, gSyncBuffer, 0xC, 0, 0);
    else
        func_00237288(D_00443800, cmd, 1, 0, 0, gSyncBuffer, 0xC, 0, 0);
    while (!snd_GotReturns__Fv())
        ;
    ret = gSyncBuffer[1];
    if (*gSndCommandBuffePtr[gCommandFillBuffer] && !gCaching)
        snd_SendCurrentBatch__Fv();
    return ret;
}

// ---- Common symbols defined here (docs/guides/tu-merging.md "Data", address names): g++ 2.95 emits a class or
// template static data member as COMMON (C: a tentative definition), and the linker allocates commons at the end
// of .bss (small ones in .sbss), which is why these addresses are not in TU order. Sizes are the gaps to the next
// name in the ELF; a 16-aligned start with a 16-multiple size is a qword array (commons take the type's alignment).
int c_00443880[3] __asm__("D_00443880");   /* .bss 0xC */
int c_0044388C[189] __asm__("D_0044388C");   /* .bss 0x2F4 */
