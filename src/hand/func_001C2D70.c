struct S { char pad[0x150]; char *p150; int x; void *p158; };
void RenderSoSelf(struct S *p);
void func_001A5EE8(void *p, void *q, float a, float b);
/** @sly2-annot addr=0x1C2D70
 * Purpose: CID 99 vt19: RenderSoSelf then place held loot object relative to attach XF (SetLoMatWorldRel)
 * Info: proposed name RenderCid99Self (T2); class CID99 slot19; TU tu_1C2D20
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1c2d20-0x1c2d20-0x1c3390
 * @end-sly2-annot */
void func_001C2D70(struct S *p)
{
    RenderSoSelf(p);
    if (p->p158) func_001A5EE8(p->p158, p->p150 + 0x40, 1.0f, 1.0f);
}
