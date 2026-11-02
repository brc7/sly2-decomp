// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
extern int D_002C4640, D_002C4680, D_002C4684, D_002C46B8;
extern int *D_002C4660[2];
extern unsigned char D_00443880[];
extern unsigned char D_0044388C[];
extern int D_00443840[];
extern char D_00443800[];
extern char D_002AA5A0[], D_002AA1C8[];
void func_0021EC18(const char *, ...);
void *func_0023320C(void *, void *, int);
void func_00231C48(void);
void func_00234CA0(int);
void func_00231F38(void *, int);
int func_00237488(void *);
int func_00237288(void *, int, int, void *, int, void *, int, void *, void *);
int func_00231E90(void);
void snd_SendCurrentBatch__Fv(void);

int func_002325E0(int cmd, int size, unsigned char *buf)
{
    int i;
    int ret;
    if (cmd != 0x68) {
        for (i = 0; i < size; i++)
            D_00443880[i] = buf[i];
    } else {
        if ((unsigned int)(*(int *)(buf + 8) + 0xC) > 0x200) {
            func_0021EC18(D_002AA5A0);
            while (1)
                ;
        }
        for (i = 0; i < 0xC; i++)
            D_00443880[i] = buf[i];
        func_0023320C(D_0044388C, *(void **)(buf + 0xC), *(int *)(buf + 8));
    }
    while (D_002C4640) {
        func_00231C48();
        func_00234CA0(0);
    }
    func_00231F38(D_00443840, 1);
    while (func_00237488(D_00443800)) {
        if (!D_002C46B8)
            func_0021EC18(D_002AA1C8);
        func_00231C48();
        func_00234CA0(0);
    }
    if (size)
        func_00237288(D_00443800, cmd, 1, D_00443880, size, D_00443840, 0xC, 0, 0);
    else
        func_00237288(D_00443800, cmd, 1, 0, 0, D_00443840, 0xC, 0, 0);
    while (!func_00231E90())
        ;
    ret = D_00443840[1];
    if (*D_002C4660[D_002C4680] && !D_002C4684)
        snd_SendCurrentBatch__Fv();
    return ret;
}
