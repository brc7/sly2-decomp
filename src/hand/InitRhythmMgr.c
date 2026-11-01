// Claude agent (lane D37): writing the range through a pointer to the 0x234 pair keeps cse from folding
// gMax - gMin, like the target (no asm, no volatile).
struct R { float g; int n; float g8; };
struct LM { float gMin; float gMax; };
struct S { char pad[0x40]; unsigned int fl; char pad2[0x14]; float g58; float g5C; char pad3[0x234 - 0x60];
    struct LM lm; char pad4[0x280 - 0x23C]; int n280; char pad5[8]; struct R r; float g298; float g29C; float g2A0; float g2A4; };
extern struct S *D_002DE570;
void InitLo(struct S *p);
void SetLoBusyk(struct S *p, int n);
/** @sly2-annot addr=0x1526D8
 * Purpose: cid 0x79 vt7 Init: base 0x1A5270; D_002DE570 = this (singleton); score range
 *   lm(+0x234)=[-1.5,1.0], windows 0.4/0.4, 8-sample average, grade thresholds
 * Info: proposed name InitRhythmMgr (T1); class CID121 slot7; TU rhythm
 * Confidence: mid (draft) [T1 0.50] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8
 * @end-sly2-annot */
void InitRhythmMgr(struct S *p)
{
    unsigned int v;
    struct R *pr;
    InitLo(p);
    D_002DE570 = p;
    {
        struct LM *plm = &p->lm;
        plm->gMin = -1.5f;
        plm->gMax = 1.0f;
    }
    p->g58 = 0.4f;
    p->g5C = 0.4f;
    p->n280 = 8;
    pr = &p->r;
    pr->g = __builtin_fabsf(p->lm.gMax - p->lm.gMin) * 0.33333334f;
    pr->g8 = 5.0f;
    pr->n = 0;
    p->g29C = -0.8f;
    p->g2A0 = -0.2f;
    p->g298 = -1.5f;
    p->g2A4 = -1.5f;
    SetLoBusyk(p, 1);
    v = p->fl;
    if ((v ^ 1) & 1) *(int *)v |= 0xA;
    else p->fl = v | 0xA;
}
