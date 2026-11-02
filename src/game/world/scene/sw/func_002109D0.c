struct X { char pad[8]; int x8; char padc[0x2C]; int x38; };
extern int D_003E1C38;
struct X *func_00210990(int i);
static inline int FEqual(struct X *a, struct X *b)
{
    if (a == b)
        return 1;
    if (!a || !b)
        return 0;
    return (a->x8 ^ a->x38) == (b->x8 ^ b->x38);
}
/** @sly2-annot addr=0x2109D0
 * Purpose: Index of level whose file record equals arg, else -1
 * Info: proposed name IlevelFromRec (T2); TU sw
 * Notes: names_v2 v1 proposed PostVolbtnLoad__FP6VOLBTN (0.004)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
int func_002109D0(struct X *px)
{
    int i;
    for (i = 0; i < D_003E1C38; i++) {
        if (FEqual(px, func_00210990(i)))
            return i;
    }
    return -1;
}
