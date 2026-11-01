struct S { char pad[0x18C8]; int n; };
void func_00143220(struct S *p, int *pf);
/** @sly2-annot addr=0x18DE80
 * Purpose: JT vt96 (p,int*pf): base 0x143220 then *pf=1 for jts 0,1,5,10,12
 * Info: proposed name JT_vt96 (T2); class JT slot96; TU jt
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_0018DE80(struct S *p, int *pf)
{
    func_00143220(p, pf);
    switch (p->n) {
    case 0: case 1: case 5: case 10: case 12:
        *pf = 1;
    }
}
