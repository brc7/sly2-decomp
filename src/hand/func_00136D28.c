typedef int ti128 __attribute__((mode(TI)));
struct E { char pad[0x18]; int f18; };
struct S { int f0; int f4; int pad[6]; ti128 v20; float f30; int f34; struct E ae[6]; };
void func_00137A30(int, int, int, struct E *, int, int, int, int *);
/** @sly2-annot addr=0x136D28
 * Purpose: Init 6 blended animation slots (EnsureAsegBlendDynamic) for an aim helper struct at +0x1710
 * Info: proposed name InitBentleyCrosshair (T2); TU bentley
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
void func_00136D28(struct S *p, int a, ti128 v, float g0, float g1) {
    unsigned int i;
    p->f0 = a;
    p->v20 = v;
    p->f30 = g1;
    p->f4 = -1;
    for (i = 0; i < 6; i++) func_00137A30(p->f0, 0xc, 2, &p->ae[i], 0, 0, 0, &p->ae[i].f18);
}
