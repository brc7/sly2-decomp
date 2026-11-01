struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[50]; };
struct E;
struct L { char pad[0xC]; struct VT *pvt; char pad1[0x24 - 0x10]; struct L *next; char pad2[0x64 - 0x28]; struct L *first; char pad3[0x70 - 0x68]; struct L *end; char pad4[0xEC - 0x74]; struct E *pe; };
struct E { char pad[0x60]; struct L *p60; };
struct DL;
extern struct DL D_003E11AC;
void func_001A6150(struct L *);
void func_001D6310(struct L *);
void func_0011A2E8(struct DL *, void *);
#define VF(q, n) (q)->pvt->a[n].pfn((char *)(q) + (q)->pvt->a[n].d)
/** @sly2-annot addr=0x1F3E00
 * Purpose: SO vt22: LO::Freeze, unregister (0x1D6310), vt22 children, if tree owner: vt49 + remove from
 *   D_003E11AC
 * Info: proposed name OnSoRemove (T1); class SO slot22; TU so
 * Notes: names_v2 C proposed RebuildSoPhysHook__FP2SO (0.638) | slot 22 OnRemove resolved by arbitration
 *   (OnRemove/OnAdd = object leaves/enters the active world, incl. proximity activation) (was FreezeSo)
 * Confidence: high [T1 0.85] - evidence: arbitration; static naming pass; sources:
 *   arbitration_slots22_23+verdicts_tu_r4r6
 * See: docs/engine/guards-ai.md#9-key-functions, docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_001F3E00(struct L *p)
{
    struct L *q;
    int unused[4];
    func_001A6150(p);
    func_001D6310(p);
    for (q = p->first; q != p->end; q = q->next) VF(q, 22);
    if (p->pe && p->pe->p60 == p) {
        VF(p, 49);
        func_0011A2E8(&D_003E11AC, p->pe);
    }
}
