struct S { char pad[0x164]; void *x; char y[4]; };
void func_001F2C30(struct S *p);
void func_001F1448(void *a, struct S *p, int b, int c, void *d);
/** @sly2-annot addr=0x224028
 * Purpose: SO slot11 then, if bound SM, 0x1F1448(sm,this,-1,0,goal)
 * Info: proposed name AddCid100Hierarchy (T2); class CID100 slot11; TU tu_223F70
 * Notes: agent qualifier: (slot11)
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/gameplay-objects.md#tu_223f70-0x223f70-0x2259d8
 * @end-sly2-annot */
void func_00224028(struct S *p)
{
    func_001F2C30(p);
    if (p->x) func_001F1448(p->x, p, -1, 0, p->y);
}
