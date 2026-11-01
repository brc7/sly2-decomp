struct S { char pad[0x150]; int x; };
/** @sly2-annot addr=0x224590
 * Purpose: returns state in {1,2}
 * Info: proposed name FCid100Active (T2); class CID100; TU tu_223F70
 * Notes: agent qualifier: state 1 or 2
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/gameplay-objects.md#tu_223f70-0x223f70-0x2259d8
 * @end-sly2-annot */
int func_00224590(struct S *p)
{
    switch (p->x) { case 1: case 2: return 1; }
    return 0;
}
