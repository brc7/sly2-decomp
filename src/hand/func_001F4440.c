typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0;
struct T { char pad[0x70]; qword v; char pad2[0x14]; int n; };
struct S { char pad[0x58]; struct T *pt; };
qword func_0022D348(struct T *, int, qword);
/** @sly2-annot addr=0x1F4440
 * Purpose: SO vt33: velocity of position XF (kind 1) in world space, else zero
 * Info: proposed name GetSoVelocity (T1); class SO slot33; TU so
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
qword func_001F4440(struct S *p) { struct T *pt = p->pt; if (!pt || pt->n != 1) return D_002A2DD0; return func_0022D348(pt, 0, pt->v); }
