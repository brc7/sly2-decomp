struct S { char pad[0x150]; int f150; };
struct M { int f0; struct S *ps; };
void HandleBasicMessage(struct S *, int, struct M *);
void PrimeBomb__FP4BOMBf(struct S *, float);
/** @sly2-annot addr=0x13AAE8
 * Purpose: BOMB vt5: base handler; on msg 4 about itself while not primed, PrimeBomb(0)
 * Info: proposed name HandleBombMessage (T1); class BOMB slot5; TU bomb
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#bomb-0x13a9e8-0x13bdf0
 * @end-sly2-annot */
void HandleBombMessage(struct S *p, int msg, struct M *pm) {
    HandleBasicMessage(p, msg, pm);
    if (msg == 4 && pm->ps == p && p->f150 == 0) PrimeBomb__FP4BOMBf(p, 0.0f);
}
