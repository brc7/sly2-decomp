struct K { int a; int b; };
struct S { char pad[0x100]; struct K k; };
void func_002311B0(struct S *p);
/** @sly2-annot addr=0x185C98
 * Purpose: vt41: CloneXfm keeping own field
 * Info: proposed name CloneCid9 (T2); class CID9 slot41; TU tu_185130
 * Notes: names_v2 C proposed CloneSpire__FP5SPIRET0 (0.438)
 * Confidence: low (draft) [T2 0.50] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/player.md#tu_185130-0x185130-0x186610
 * @end-sly2-annot */
void func_00185C98(struct S *p)
{
    struct K k = p->k;
    func_002311B0(p);
    p->k = k;
}
