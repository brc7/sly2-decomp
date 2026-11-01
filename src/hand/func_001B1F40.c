struct Q { char pad[0x18]; int x18; };
struct S { char pad[0x238]; int c238; int a23C[4]; int c24C; int a250[4]; };
int func_0013BBF8(struct S *p, struct Q *q);
int func_00134310(int r, int n);
int func_00132B10(struct Q *q, int k);
/** @sly2-annot addr=0x1B1F40
 * Purpose: MISSILE vt61: BOMB base result, modified when other SO's oid is in list +0x23C or derives from a
 *   cid in list +0x250
 * Info: proposed name FIgnoreMissileIntersection (T1); class MISSILE slot61; TU missile
 * Notes: MISSILE vtable slot 61, missile TU (verifier better name; r5 placeholder MissileVt61CollideFilter)
 *   (was MissileVt61CollideFilter)
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: verify_A2+verdicts_A2
 * See: docs/engine/gameplay-objects.md#gameplay-object-classes,
 *   docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#missile-0x1b1db8-0x1b2820
 * @end-sly2-annot */
int func_001B1F40(struct S *p, struct Q *q)
{
    int i, j;
    int r = func_0013BBF8(p, q);
    for (i = 0; i < p->c238; i++)
        if (p->a23C[i] == q->x18) r = func_00134310(r, 1);
    for (j = 0; j < p->c24C; j++)
        if (func_00132B10(q, p->a250[j])) r = func_00134310(r, 1);
    return r;
}
