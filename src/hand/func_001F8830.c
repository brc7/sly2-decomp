struct T { char pad[0xAE]; short n; };
struct S { char pad[0x20]; void *p; struct T *pt; };
void func_00230598(void *, struct T *);
/** @sly2-annot addr=0x1F8830
 * Purpose: XF +0xAE = g*1000, refresh
 * Info: proposed name SetXfhParamAE (T2); TU so
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F8830(struct S *p, float g) { p->pt->n = (int)(g * 1000.0f); func_00230598(p->p, p->pt); }
