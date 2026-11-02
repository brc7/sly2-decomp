struct U { char pad[0x8]; float g; };
struct T { char pad[0xDC]; struct U *pu; };
struct S { char pad[0xE8]; struct T *pt; };
/** @sly2-annot addr=0x1F83E0
 * Purpose: record.g8 or 0
 * Info: proposed name GetSoPhysParam8 (T2); TU so
 * Notes: names_v2 v1 proposed AddSoXa__FP2SOP2XA (0.001)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
float func_001F83E0(struct S *p) { struct U *pu = 0; if (p->pt) pu = p->pt->pu; if (pu) return pu->g; return 0.0f; }
