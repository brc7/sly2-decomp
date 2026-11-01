typedef int qword __attribute__((mode(TI)));
struct S8 { float a, b; };
struct X {
    char pad[0x11C4];
    float f11C4, f11C8;
    struct S8 s11CC;
    float f11D4, f11D8;
    qword q11E0;
    char pad2[0x1280 - 0x11F0];
    qword q1280;
};
/** @sly2-annot addr=0x142310
 * Purpose: Splice: store throw parameters (4 floats, an LM, two vectors) in jtthrow (+0x11C4..+0x11E0,
 *   +0x1280)
 * Info: proposed name SetStepplayerThrowParams (T2); TU stepplayer
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00142310(struct X *p, struct S8 *ps, qword q1, qword q2, float a, float b, float c, float d)
{
    p->f11C4 = a;
    p->f11C8 = b;
    p->s11CC = *ps;
    p->f11D4 = c;
    p->f11D8 = d;
    p->q1280 = q1;
    p->q11E0 = q2;
}
