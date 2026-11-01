
struct S { char pad[0xE8]; int c; };
float UMaxShape(struct S *p);
float UMinShape(struct S *p);
/** @sly2-annot addr=0x1E77E8
 * Purpose: (uMax-uMin)/(count*10)
 * Info: proposed name DuGetShapeSearchIncrement (T1); TU shape
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
float DuGetShapeSearchIncrement(struct S *p)
{
    float a = UMaxShape(p);
    return (a - UMinShape(p)) / (p->c * 10.0f);
}
