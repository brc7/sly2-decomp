struct Q { char b[8]; };
struct S { char pad[0xE0]; struct Q q; };
void func_002311B0(struct S *p);
/** @sly2-annot addr=0x21D498
 * Purpose: same as CloneTarget for cid 0x15 (preserve DLE at 0xE0)
 * Info: proposed name CloneCid21 (T2); class CID21 slot41; TU target
 * Confidence: low (draft) [T2 0.85] - evidence: static analysis; sources: r8_21BB88+verdicts_tu_r7r8
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#target-0x21d278-0x21d638
 * @end-sly2-annot */
void func_0021D498(struct S *p)
{
    struct Q t = p->q;
    func_002311B0(p);
    p->q = t;
}
