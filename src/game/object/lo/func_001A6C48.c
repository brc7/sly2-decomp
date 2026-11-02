typedef unsigned long ulong;
struct G;
extern struct G D_003DD490;
void func_00147E88(struct G *g, void *p);
struct S { char pad[0x10]; int x10; void *x14; char pad2[0x10]; ulong fl; int pad3[2]; float x38; };
/** @sly2-annot addr=0x1A6C48
 * Purpose: LO vt42: store float x38 (init 1e10) and re-sort root in g_sw busy list
 * Info: proposed name SetLoSBusy (T2); class LO slot42; TU lo
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void func_001A6C48(struct S *p, float g)
{
    p->x38 = g;
    if (!p->x10 && ((int)(p->fl >> 60) & 1)) func_00147E88(&D_003DD490, p->x14);
}
