struct S { char pad[0x40]; int f40; char pad2[0x148 - 0x44]; unsigned long grf; };
void PostSoLoad(struct S *p);
void SetLoBusyk(struct S *p, int n);
/** @sly2-annot addr=0x1EF850
 * Purpose: SKY (CID 0x61) vt15: PostSoLoad, busyk 1, grf|=2, clear +0x148 bit35 (never in sphere tree)
 * Info: proposed name PostSkyLoad (T1); class SKY slot15; TU sky
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/gameplay-objects.md#gameplay-object-classes, docs/engine/guards-ai.md#8-source-files,
 *   docs/engine/rendering.md#12-source-files
 * @end-sly2-annot */
void func_001EF850(struct S *p)
{
    int x;
    PostSoLoad(p);
    SetLoBusyk(p, 1);
    x = p->f40;
    if ((x ^ 1) & 1) *(int *)x |= 2;
    else p->f40 = x | 2;
    p->grf &= ~(1UL << 35);
}
