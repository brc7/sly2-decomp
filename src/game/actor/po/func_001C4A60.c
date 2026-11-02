struct LO;
struct VTE { short d; short pad; void (*pf)(); };
struct LO { char pad[0xC]; struct VTE *vt; };
struct G { char pad[0x3EFC]; struct LO *plo3EFC; struct LO *plo3F00; };
struct LO *PpoCur(struct G *p);
#define VCALL(o, n, a) ((void (*)(void *, int))(o)->vt[n].pf)((char *)(o) + (o)->vt[n].d, a)
/** @sly2-annot addr=0x1C4A60
 * Purpose: Set override PO (SW+0x3F00) with OnActive(0/1) on old/new effective PO
 * Info: proposed name SetSwPoOverride (T2); TU po
 * Notes: names_v2 C proposed RopsNextRop__FP3ROP (0.638)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/engine/world-sw.md#10-key-functions, docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_001C4A60(struct G *p, struct LO *plo)
{
    struct LO *cur = PpoCur(p);
    struct LO *nw = plo;
    if (!nw) nw = p->plo3EFC;
    if (cur != nw && cur) VCALL(cur, 78, 0);
    p->plo3F00 = plo;
    if (cur != nw && nw) VCALL(nw, 78, 1);
}
