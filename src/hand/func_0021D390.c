struct Q { char b[8]; };
struct S { char pad[0xE0]; struct Q q; };
void func_002311B0(struct S *p);
/** @sly2-annot addr=0x21D390
 * Purpose: saves the 8-byte DLE at 0xE0, CloneXfm (0x2311B0), restores DLE
 * Info: proposed name CloneTarget (T1); class TARGET slot41; TU target
 * Notes: names_v2 C proposed BindHshape__FP6HSHAPE (0.638)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r8_21BB88+verdicts_tu_r7r8
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#target-0x21d278-0x21d638
 * @end-sly2-annot */
void func_0021D390(struct S *p)
{
    struct Q t = p->q;
    func_002311B0(p);
    p->q = t;
}
