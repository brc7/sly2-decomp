typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
struct P { char pad[0x10E4]; int x10E4; void *p10E8; int x10EC; };
extern float D_002BCC7C, D_002BCC80, D_002BCC84, D_002BCC88, D_002BCC8C, D_002BCC90;
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int n);
qword func_001A62E8(void *p);
/** @sly2-annot addr=0x140AF8
 * Purpose: True when interact mode (+0x10EC)==4 and kind!=3 and the target (+0x10E8) is within an xy distance
 *   (larger if target has class 0x3A) and a z window, widened when fX
 * Info: proposed name FStepplayerNearInteractTarget (T1); TU stepplayer
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
int FStepplayerNearInteractTarget(struct P *p, int fX)
{
    float gDist, gMin, gMax;
    qword a, b, t;
    U d;
    float g;
    int f;
    if (p->x10EC != 4 || p->x10E4 == 3) return 0;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(p->p10E8, 0x3A)) gDist = D_002BCC7C;
    else gDist = D_002BCC80;
    gMin = D_002BCC84;
    gMax = D_002BCC88;
    if (fX) {
        gDist += D_002BCC8C;
        gMin -= D_002BCC90;
        gMax += D_002BCC90;
    }
    a = func_001A62E8(p->p10E8);
    b = func_001A62E8(p);
    f = 0;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d.q) : "j"(a), "j"(b));
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(t) : "j"(d.q));
    __asm__("vsqrt Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "+j"(t));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(t));
    if (g < gDist && gMin < d.f[2] && d.f[2] < gMax) f = 1;
    return f;
}
