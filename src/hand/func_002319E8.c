// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef struct CB { void (*fn)(int, long); int pad; long arg; } CB;
typedef struct RPC { int x[16]; } RPC;
extern int *D_002C4660[2];
extern int D_002C4668[2];
extern CB *D_002C4670[2];
extern int *D_002C4678[2];
extern int D_002C4688, D_002C46B8;
extern CB D_002C4690;
extern int D_00443B80[], D_00444B80[], D_00447B80[], D_00447FC0[];
extern CB D_00445B80[], D_00446B80[];
extern RPC D_00443800, D_00448440;
extern int D_00448400[];
extern unsigned int D_00448480;
extern char D_002AA100[], D_002AA128[];
void func_0021EC18(const char *, ...);
void func_00236A60(int);
int func_002370A8(void *, int, int);
int func_002325E0(int, int, void *);

/** @sly2-annot addr=0x2319E8
 * Purpose: 989snd API
 * Info: proposed name snd_StartSoundSystem (T1); TU 989snd
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r8_21BB88+verdicts_tu_r7r8+verdicts_A2
 * See: docs/engine/object-model.md#8-source-files, docs/engine/overview.md#0-the-shape-of-the-program,
 *   docs/engine/sound.md#5-source-files
 * @end-sly2-annot */
void func_002319E8(int flags)
{
    int i;
    int buf[2];

    D_002C4660[0] = D_00443B80;
    D_002C4660[1] = D_00444B80;
    D_002C4678[0] = D_00447B80;
    D_002C4678[1] = D_00447FC0;
    D_002C4670[0] = D_00445B80;
    D_002C4670[1] = D_00446B80;
    if (flags & 2)
        D_002C46B8 = 1;
    else
        D_002C46B8 = 0;
    func_00236A60(0);
    do {
        if (func_002370A8(&D_00443800, 0x123456, 0) < 0) {
            func_0021EC18(D_002AA100, D_002AA128, 0xB5);
            while (1)
                ;
        }
        i = 9999;
        while (i--)
            ;
    } while (!D_00443800.x[9]);
    D_002C4688 = 0;
    D_002C4690.fn = 0;
    D_002C4690.arg = 0;
    D_00448480 = 0;
    do {
        if (func_002370A8(&D_00448440, 0x123457, 0) < 0) {
            func_0021EC18(D_002AA100, D_002AA128, 0xCA);
            while (1)
                ;
        }
        i = 9999;
        while (i--)
            ;
    } while (!D_00448440.x[9]);
    ((volatile int *)D_00448400)[0] = 0;
    ((volatile int *)D_00448400)[4] = 0;
    *D_00443B80 = 0;
    *D_00444B80 = 0;
    D_002C4668[0] = 0xFFC;
    buf[0] = (int)D_00448400;
    buf[1] = flags;
    D_002C4668[1] = 0xFFC;
    func_002325E0(0, 8, buf);
}
