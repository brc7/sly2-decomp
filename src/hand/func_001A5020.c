struct B { int x0; int x4; char pad[0x34]; int x3c; };
struct R { int x0; struct B *x4; };
struct R *func_001A4FB0(void *a, int n);
static inline int F(struct B *b) { if (!b) return 0; return b->x4 != b->x3c; }
/** @sly2-annot addr=0x1A5020
 * Purpose: If valid, queue kind 0 with movie pointer
 * Info: proposed name AddBootMovie (T2); TU tu_1A4E08
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/world-sw.md#tu_1a4e08-0x1a4e08-0x1a5270
 * @end-sly2-annot */
void func_001A5020(void *a, struct B *b)
{
    if (F(b)) func_001A4FB0(a, 0)->x4 = b;
}
