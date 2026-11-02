struct S { char pad[0x10]; int x10; int x14; char p2[0x24]; int x3c; char p3[0x10]; int x50; };
void func_00184C60(struct S *p);
void func_001849A0(struct S *p);
/** @sly2-annot addr=0x184CA0
 * Purpose: Virtual Refill: host file if fd>=0, CD if sector stream active, else empty
 * Info: proposed name CStreamBis_Refill (T1); class CStreamBis; TU tu_1848A8
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/core-library.md#tu_1848a8-0x1848a8-0x184f50
 * @end-sly2-annot */
void CStreamBis_Refill(struct S *p)
{
    if (p->x3c >= 0) func_00184C60(p);
    else if (p->x50 >= 0) func_001849A0(p);
    else {
        p->x10 = 0;
        p->x14 = 0;
    }
}
