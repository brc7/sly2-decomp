struct T { char pad[0xAE]; unsigned short n; };
struct S { char pad[0x24]; struct T *pt; };
/** @sly2-annot addr=0x1F8808
 * Purpose: XF u16 +0xAE * 0.001
 * Info: proposed name GetXfhParamAE (T2); TU so
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
float func_001F8808(struct S *p) { return p->pt->n * 0.001f; }
