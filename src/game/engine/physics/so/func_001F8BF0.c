typedef int qword __attribute__((mode(TI)));
struct U { int a, b; int n; float x, y, z, r; int pad; qword q; };
struct V { char pad[0x60]; struct U *pu; };
struct T { char pad[0x98]; short k; char pad2[0xE]; struct V *pv; };
struct S { char pad[0x24]; struct T *pt; };
/** @sly2-annot addr=0x1F8BF0
 * Purpose: For XF kind 16: set n, xyz, angle (deg->rad), q
 * Info: proposed name SetXfhKind16Params (T2); TU so
 * Notes: names_v2 v1 proposed PsoFindSoPhysHook__FP2SOi (0.051)
 * Confidence: low (draft) [T2 0.25] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F8BF0(struct S *p, int n, qword q, float x, float y, float z, float deg)
{ struct T *pt = p->pt; if (pt->k == 16) { struct U *pu = pt->pv->pu; pu->n = n; pu->x = x; pu->y = y; pu->z = z; pu->r = deg * 0.01745329424738884f; pu->q = q; } }
