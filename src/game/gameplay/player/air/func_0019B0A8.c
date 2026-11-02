struct SO { char pad[0x70]; long fl; };
struct A { char pad[0xEC]; struct SO *pso; };
struct X { int a; struct A *pa; int b, c; long fl; };
struct L { int c; struct SO **a; };
/** @sly2-annot addr=0x19B0A8
 * Purpose: Obstruction filter: ignore flagged XPs, objects with flag 4 and objects in an ignore list
 * Info: proposed name FFilterJtObstruction (T2); TU air
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
int func_0019B0A8(struct L *l, struct X *x) {
    struct SO *pso;
    int i;
    if (x->fl & (1L << 48)) return 0;
    pso = x->pa->pso;
    if (pso->fl & 4) return 0;
    for (i = 0; i < l->c; i++) if (pso == l->a[i]) return 0;
    return 1;
}
