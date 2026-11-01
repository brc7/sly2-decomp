typedef unsigned long ulong;
struct DL { void *a, *b; int c; };
extern struct DL D_003DD4BC;
int func_0011A380(struct DL *pdl, void *pv);
struct S { char pad[0x14]; void *x14; char pad2[0x10]; ulong fl; };
/** @sly2-annot addr=0x1A61A0
 * Purpose: bit60 set and plo->ploroot is in g_sw+0x2C list
 * Info: proposed name FIsLoInWorld (T1); TU lo
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/object-model.md#4-activation-onadd--onremove-slots-23--22,
 *   docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
int func_001A61A0(struct S *p)
{
    int r = 0;
    if ((int)(p->fl >> 60) & 1) r = func_0011A380(&D_003DD4BC, p->x14) != 0;
    return r;
}
