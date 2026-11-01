typedef struct { short d; short pad; void *pfn; } VTE;
struct O { char pad[0xC]; VTE *vt; };
struct S { char pad[0x2C0]; long fl; char pad2[0xFD0 - 0x2C8]; struct O *po; };
long GrfFromSgf(int n);
int func_002040F8(struct S *p);
/** @sly2-annot addr=0x20B090
 * Purpose: STEPGUARD vt85: 1 if flag 0x14; 0 if flags 0x12/0x13/0x16; else mover driver vt12 or STEP base
 * Info: proposed name GrfStepguardGroundProbe (T2); class STEPGUARD slot85; TU stepguard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int func_0020B090(struct S *p)
{
    long m;
    if (p->fl & GrfFromSgf(0x14))
        return 1;
    m = GrfFromSgf(0x12) | GrfFromSgf(0x13) | GrfFromSgf(0x16);
    if (p->fl & m)
        return 0;
    if (p->po)
        return ((int (*)(void *))p->po->vt[12].pfn)((char *)p->po + p->po->vt[12].d);
    return func_002040F8(p);
}
