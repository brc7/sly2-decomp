// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
extern int D_002C464C;
extern int D_002C4658;
extern int D_00448400[];
int func_00241E88(void);
void func_00235078(void *, void *);
void func_00231C48(void);

/** @sly2-annot addr=0x232DA8
 * Purpose: 989snd API
 * Info: proposed name snd_StreamSafeCdSync (T1); TU 989snd
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r8_21BB88+verdicts_A2
 * See: docs/engine/sound.md#6-key-functions, docs/reference/functions/sound.md#989snd-0x2319e8-0x233000
 * @end-sly2-annot */
int func_00232DA8(int mode)
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
        func_00231C48();
        func_00235078(D_00448400, pe);
        D_002C4658 = (D_00448400[0] == 0);
    }
    return 0;
}
