void *func_001E7840(void *p, int n);
void func_001E93F0(void *p, int a, int b);
/** @sly2-annot addr=0x1E7968
 * Purpose: Chain end 1 follows object (or shape end)
 * Info: proposed name SetShapeChainEndObject (T2); TU shape
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void func_001E7968(void *p, int b)
{
    func_001E93F0(func_001E7840(p, 1), 1, b);
}
