struct T { char pad[0xAC]; short n; };
struct S { char pad[0x20]; void *p; struct T *pt; };
void SlotUnsettle(void *, struct T *);
/** @sly2-annot addr=0x1F8898
 * Purpose: duplicate of 0x1F87C8
 * Info: proposed name SetXfhParamAC2 (T2); TU so
 * Confidence: low (draft) [T2 0.15] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F8898(struct S *p, float g) { p->pt->n = (int)(g * 1000.0f); SlotUnsettle(p->p, p->pt); }
