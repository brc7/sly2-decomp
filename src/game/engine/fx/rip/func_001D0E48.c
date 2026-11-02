typedef int qword __attribute__((mode(TI)));
union U { qword q; float a[4]; };
struct S {
    char pad[0x20]; float f20;
    char pad2[0xA0 - 0x24]; qword qA0;
    char pad3[0xC4 - 0xB0]; float fC4;
};
extern qword D_002C1F40;
extern void func_001CE920(struct S *);

/** @sly2-annot addr=0x1D0E48
 * Purpose: RIPT 0 Init: InitRip, scale 0.33, v = -0.2*D_002C1F40, accel -8
 * Info: proposed name InitBublet (T2); TU rip
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void func_001D0E48(struct S *p)
{
    union U u;
    qword k, r;
    func_001CE920(p);
    p->f20 = 0.33f;
    __asm__("qmtc2.ni %1, %0" : "=j"(k) : "r"(-0.2f));
    u.q = k;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(D_002C1F40), "j"(k));
    p->qA0 = r;
    p->fC4 = -8.0f;
}
