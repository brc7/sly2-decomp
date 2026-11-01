struct E { int k; int a[6]; };
struct SO { int x0; int x4; char pad[0x34]; int x3c; };
struct E *func_001A4FB0(void *pl, int k);
static inline int FSo(struct SO *pso) { if (!pso) return 0; return pso->x4 != pso->x3c; }
/** @sly2-annot addr=0x1A5078
 * Purpose: Queue kind 2 {so, oid, flag}
 * Info: proposed name AddBootWorldWarp (T2); TU tu_1A4E08
 * Notes: names_v2 C proposed InitSwAoxa__FP2SW (0.200)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/world-sw.md#tu_1a4e08-0x1a4e08-0x1a5270
 * @end-sly2-annot */
void func_001A5078(void *pl, struct SO *pso, int a2, int a3)
{
    if (FSo(pso)) {
        struct E *pe = func_001A4FB0(pl, 2);
        pe->a[1] = (int)pso;
        pe->a[2] = a2;
        pe->a[3] = a3;
    }
}
