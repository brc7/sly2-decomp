struct B { char b[0x30]; };
struct VTE { short d; short pad; void (*pfn)(void *, int, float, float, struct B *); };
struct O { char pad[0xC]; struct VTE *pvt; char pad2[0x24 - 0x10]; struct O *pNext; };
struct P { char pad[0x64]; struct O *pFirst; char pad2[0x70 - 0x68]; struct O *pEnd; char pad3[0x80 - 0x74]; char x80[0x134 - 0x80]; struct B *pb; };
struct G;
extern struct G D_003DD490;
void func_001A4478(struct G *, int, struct B *);
void RenderSoGlobset(void *, int, float, float, struct B *);
/** @sly2-annot addr=0x1F3CC8
 * Purpose: SO vt20: draw globset with explicit params and recurse vt20 to children
 * Info: proposed name RenderSoAll (T2); class SO slot20; TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F3CC8(struct P *p, int a, struct B *pb, float f1, float f2)
{
    struct B b;
    int unused[4];
    struct O *po;
    if (!pb && p->pb) {
        b = *p->pb;
        pb = &b;
        func_001A4478(&D_003DD490, a, &b);
    }
    RenderSoGlobset(p->x80, a, f1, f2, pb);
    for (po = p->pFirst; po != p->pEnd; po = po->pNext)
        po->pvt[20].pfn((char *)po + po->pvt[20].d, a, f1, f2, pb);
}
