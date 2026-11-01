typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0;
struct T { char pad[0x80]; qword v; };
struct S { char pad[0xE8]; struct T *pt; };
/** @sly2-annot addr=0x1F5860
 * Purpose: SOPHYS+0x80 (gravity vector) or zero
 * Info: proposed name GetSoDvGravity (T1); TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
qword GetSoDvGravity(struct S *p) { if (!p->pt) return D_002A2DD0; return p->pt->v; }
