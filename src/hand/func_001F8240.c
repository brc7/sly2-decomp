struct T { int a[3]; int n; char pad[0x10]; struct T *pnext; };
extern struct T *D_003E145C;
/** @sly2-annot addr=0x1F8240
 * Purpose: Search D_003E145C list by id
 * Info: proposed name PxxFindGlobalById (T2); TU so
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
struct T *func_001F8240(int n) { struct T *pt; for (pt = D_003E145C; pt; pt = pt->pnext) if (pt->n == n) return pt; return 0; }
