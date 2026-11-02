// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
extern int D_002C4640, D_002C4680, D_002C4684, D_002C46B8;
extern int *D_002C4660[2];
extern unsigned char D_00443880[];
extern unsigned char D_0044388C[];
extern int D_00443840[];
extern char D_00443800[];
extern char D_002AA5A0[], D_002AA1C8[];
void printf(const char *, ...);
void *func_0023320C(void *, void *, int);
void snd_FlushSoundCommands(void);
void func_00234CA0(int);
void snd_PrepareReturnBuffer(void *, int);
int func_00237488(void *);
int func_00237288(void *, int, int, void *, int, void *, int, void *, void *);
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
    while (D_002C4640) {
        snd_FlushSoundCommands();
        func_00234CA0(0);
    }
    snd_PrepareReturnBuffer(D_00443840, 1);
    while (func_00237488(D_00443800)) {
        if (!D_002C46B8)
            printf(D_002AA1C8);
        snd_FlushSoundCommands();
        func_00234CA0(0);
    }
    if (size)
        func_00237288(D_00443800, cmd, 1, D_00443880, size, D_00443840, 0xC, 0, 0);
    else
        func_00237288(D_00443800, cmd, 1, 0, 0, D_00443840, 0xC, 0, 0);
    while (!snd_GotReturns__Fv())
        ;
    ret = D_00443840[1];
    if (*D_002C4660[D_002C4680] && !D_002C4684)
        snd_SendCurrentBatch__Fv();
    return ret;
}
