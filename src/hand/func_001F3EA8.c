typedef unsigned long ulong;
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[50]; };
struct E;
struct L { char pad[0xC]; struct VT *pvt; char pad1[0x24 - 0x10]; struct L *next; char pad1b[0x40 - 0x28]; int v40; char pad2[0x64 - 0x44]; struct L *first; char pad3[0x70 - 0x68]; struct L *end; char pad4[0xEC - 0x74]; struct E *pe; char pad5[0x148 - 0xF0]; ulong fl; };
struct E { char pad[0x60]; struct L *p60; };
struct DL;
extern struct DL D_003E11AC;
void func_001A6178(struct L *);
void func_001D6378(struct L *);
int func_0011A380(struct DL *, void *);
void func_0011A220(struct DL *, void *);
#define VF(q, n) (q)->pvt->a[n].pfn((char *)(q) + (q)->pvt->a[n].d)
/** @sly2-annot addr=0x1F3EA8
 * Purpose: SO vt23: LO::Unfreeze, re-register, vt23 children, add tree to D_003E11AC and vt49
 * Info: proposed name OnSoAdd (T1); class SO slot23; TU so
 * Notes: names_v2 C proposed FGetSoContactList__FP2SOPv (0.167) | slot 23 OnAdd resolved by arbitration
 *   (OnRemove/OnAdd = object leaves/enters the active world, incl. proximity activation) (was UnfreezeSo)
 * Confidence: high [T1 0.85] - evidence: arbitration; static naming pass; sources: arbitration_slots22_23
 * See: docs/engine/guards-ai.md#9-key-functions, docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_001F3EA8(struct L *p)
{
    struct L *q;
    int unused[4];
    func_001A6178(p);
    if (p->fl & (1L << 35)) {
        int v = p->v40;
        if ((v ^ 1) & 1) v = *(int *)v;
        if (v & 8) func_001D6378(p);
    }
    for (q = p->first; q != p->end; q = q->next) VF(q, 23);
    if (p->pe && p->pe->p60 == p) {
        if (!func_0011A380(&D_003E11AC, p->pe)) func_0011A220(&D_003E11AC, p->pe);
        VF(p, 49);
    }
}
