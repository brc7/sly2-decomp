struct S { char pad[0x17C]; int n; };
void CloneXfm(struct S *p);
void func_001E7840(struct S *p, int n);
/** @sly2-annot addr=0x1E5DB8
 * Purpose: SHAPE::Clone (vt41): CloneXfm; if a chain was attached (+0x17C) rebuild it
 * Info: proposed name CloneShape (T1); class SHAPE slot41; TU shape
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void CloneShape(struct S *p)
{
    CloneXfm(p);
    if (p->n) {
        p->n = 0;
        func_001E7840(p, 0);
    }
}
