typedef int qword __attribute__((mode(TI)));
struct P;
struct S { struct P *a; int pad[3]; qword q; };
void func_001CC1F8(struct P *a, qword q, float *projection, float *distance, float t);
/** @sly2-annot addr=0x1CC2B8
 * Purpose: Parameter callback for curve B
 * Info: proposed name GJtHandsCurveB (T2); TU jthands
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#jthands-0x1cc158-0x1cd5f0
 * @end-sly2-annot */
float func_001CC2B8(struct S *p, float t) { float g; func_001CC1F8(p->a, p->q, 0, &g, t); return g; }
