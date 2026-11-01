struct S { char pad[0x15C]; int f; };
void AddSoHierarchy(struct S *p);
void func_0021BC08(struct S *p);
/** @sly2-annot addr=0x21BBD0
 * Purpose: calls SO slot11 0x1F2C30 then, if flag 0x15C set, resets the matrix ring (0x21BC08)
 * Info: proposed name Cid105_slot11 (T2); class CID105 slot11; TU trail
 * Notes: agent qualifier: (AddHierarchy override) - reset history on add
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/particles-effects.md#trail-0x21bb88-0x21c820
 * @end-sly2-annot */
void func_0021BBD0(struct S *p)
{
    AddSoHierarchy(p);
    if (p->f)
        func_0021BC08(p);
}
