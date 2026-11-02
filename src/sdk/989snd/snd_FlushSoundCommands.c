// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef struct CB { void (*fn)(int, long); int pad; long arg; } CB;
extern int D_002C4640;
extern void (*D_002C4650)(int);
extern int D_002C4654, D_002C4658, D_002C465C, D_002C4680, D_002C4684, D_002C4688;
extern int *D_002C4660[2];
extern CB *D_002C4670[2];
extern int *D_002C4678[2];
extern void (*D_002C46A4)(int, long);
extern long D_002C46A8;
extern CB D_002C4690;
extern int D_00443840[];
extern unsigned int D_00448480;
int func_00231E90(void);
void func_00234CA0(int);
void snd_SendCurrentBatch__Fv(void);
int func_00232DA8(int);

int snd_FlushSoundCommands(void)
{
    int i;
    if (D_002C4640 && func_00231E90()) {
        if (D_002C465C) {
            if (D_002C46A4)
                D_002C46A4(D_00443840[1], D_002C46A8);
            D_002C46A4 = 0;
            D_002C465C = 0;
        } else {
            int other = D_002C4680 != 1;
            for (i = 0; i < *D_002C4660[other]; i++) {
                if (D_002C4670[other][i].fn)
                    D_002C4670[other][i].fn(D_002C4678[other][i + 1], D_002C4670[other][i].arg);
            }
        }
    }
    if (D_002C4688) {
        func_00234CA0(0);
        if (D_00448480 != 0xFFFFFFFF) {
            void (*fn)(int, long) = D_002C4690.fn;
            if (fn) {
                long arg = D_002C4690.arg;
                D_002C4690.fn = 0;
                D_002C4690.arg = 0;
                fn(D_00448480, arg);
            }
            D_00448480 = 0;
            D_002C4688 = 0;
        }
    }
    if (!D_002C4640) {
        if (*D_002C4660[D_002C4680] && !D_002C4684)
            snd_SendCurrentBatch__Fv();
    }
    if (D_002C4654) {
        func_00232DA8(1);
        if (D_002C4658) {
            D_002C4654 = 0;
            D_002C4658 = 0;
            if (D_002C4650)
                D_002C4650(1);
        }
    }
    if (D_002C4640 || D_002C4688)
        return 1;
    return 0;
}
