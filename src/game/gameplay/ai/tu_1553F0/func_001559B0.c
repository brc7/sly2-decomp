struct S { int pad[4]; int n10; int pad2[7]; int n30; int pad3[2]; float g3C; };
extern float D_002A5134;
void InitBhv(struct S *p);
void AddStepguardSgf(int n, int k);
/** @sly2-annot addr=0x1559B0
 * Purpose: BHV181 slot 7: base, flag 0x23, state -1
 * Info: proposed name InitBhv181 (T2); class BHV181 slot7; TU tu_1553F0
 * Notes: superseded: OnBhv181Push (r3b_rest) | BHV vtable slot 7 role per r9 (CONFLICTS #15); was
 *   OnBhv181Push
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r3b_rest+bhv-slot-canon(r9)
 * See: docs/reference/functions/guards-ai.md#tu_1553f0-0x1553f0-0x1564e8
 * @end-sly2-annot */
void func_001559B0(struct S *p)
{
    InitBhv(p);
    AddStepguardSgf(p->n10, 0x23);
    p->n30 = -1;
    p->g3C = D_002A5134;
}
