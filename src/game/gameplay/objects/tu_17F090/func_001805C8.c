struct S { char pad[0x58]; int a; int b; char p2[0x178]; int x1d8; char p3[0x44]; char x220[4]; };
void func_0017F138(struct S *p);
int func_001E94B0(int a, int b);
void func_002303A0(struct S *p, int a, int b);
void func_0017F408(struct S *p, int n, void *pv);
/** @sly2-annot addr=0x1805C8
 * Purpose: vt15: PostCid47Load + bind entries
 * Info: proposed name PostHackenemyLoad (T2); class HACKENEMY slot15; TU tu_17F090
 * Notes: names_v2 C proposed SetStepguardPathzone__FP9STEPGUARD3OID (0.638) | Cid49->Cid73 (CID 0x49=73,
 *   vtable) | class identity from whole_game WE names/object strings: PostCid73Load -> PostHackenemyLoad
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r4b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_001805C8(struct S *p)
{
    func_0017F138(p);
    p->x1d8 = func_001E94B0(0x2C9, 0x10);
    if (p->a) func_002303A0(p, p->a, 2);
    if (p->b) func_002303A0(p, p->b, 2);
    func_0017F408(p, 5, p->x220);
}
