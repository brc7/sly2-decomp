void *func_001E7840(void *p, int n);
void func_001E95B0(void *p, float g);
/** @sly2-annot addr=0x1E79F0
 * Purpose: Ensure chain then append sample 0x1E95B0(chain,g)
 * Info: proposed name AddShapeChainSample (T2); TU shape
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void func_001E79F0(void *p, float g)
{
    func_001E95B0(func_001E7840(p, 1), g);
}
