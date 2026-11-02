// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
extern "C" {
extern int D_002C4680, D_002C46B8;
extern int *D_002C4660[2];
extern int D_002C4668[2];
extern void *D_002C4678[2];
extern char D_00443800[];
extern char D_002AA1C8[];
void func_00231F38(void *, int, int *);
void func_0021EC18(const char *, ...);
void func_00234CA0(int);
int func_00237488(void *);
int func_00237288(void *, int, int, void *, int, void *, int, void *, void *);
}

void snd_SendCurrentBatch()
{
    func_00231F38(D_002C4678[D_002C4680], *D_002C4660[D_002C4680], D_002C4660[D_002C4680]);
    while (func_00237488(D_00443800)) {
        if (!D_002C46B8)
            func_0021EC18(D_002AA1C8);
        func_00234CA0(0);
    }
    func_00237288(D_00443800, 0x4D, 1, D_002C4660[D_002C4680], 0x1000 - D_002C4668[D_002C4680],
                  D_002C4678[D_002C4680], *D_002C4660[D_002C4680] * 4 + 8, 0, 0);
    int c = D_002C4680 != 1;
    D_002C4680 = c;
    *D_002C4660[c] = 0;
    D_002C4668[c] = 0xFFC;
}
