struct T { char pad[0xAC]; short n; };
struct S { char pad[0x20]; void *p; struct T *pt; };
void SlotUnsettle(void *, struct T *);
/** @sly2-annot addr=0x1F87C8
 * Purpose: XF +0xAC = g*1000, refresh XF kind
 * Info: proposed name SetXfhParamAC (T2); TU so
 * Notes: names_v2 v1 proposed SetSoNoGravity__FP2SOi (0.077)
 * Confidence: low (draft) [T2 0.20] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F87C8(struct S *p, float g) { p->pt->n = (int)(g * 1000.0f); SlotUnsettle(p->p, p->pt); }
