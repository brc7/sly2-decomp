typedef int qword __attribute__((mode(TI)));
struct S { char pad[0x1880]; int n; char p1[0xC]; qword aq[3]; char p2[0xEC]; float g; };
void UpdateSoTreeBounds(struct S *p);
void ExtendSoTreeBounds(struct S *p, qword q, float g);
/** @sly2-annot addr=0x18A2E0
 * Purpose: JT vt49: SO 0x1F50E0 then 0x1F5480(pjt, q[i], g) for each extra entry
 * Info: proposed name JT_vt49 (T2); class JT slot49; TU jt
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_0018A2E0(struct S *p)
{
    int i;
    UpdateSoTreeBounds(p);
    for (i = 0; i < p->n; i++)
        ExtendSoTreeBounds(p, p->aq[i], p->g);
}
