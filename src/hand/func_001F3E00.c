struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[50]; };
struct E;
struct L { char pad[0xC]; struct VT *pvt; char pad1[0x24 - 0x10]; struct L *next; char pad2[0x64 - 0x28]; struct L *first; char pad3[0x70 - 0x68]; struct L *end; char pad4[0xEC - 0x74]; struct E *pe; };
struct E { char pad[0x60]; struct L *p60; };
struct DL;
extern struct DL D_003E11AC;
void func_001A6150(struct L *);
void func_001D6310(struct L *);
void RemoveDlEntry__FP2DLPv(struct DL *, void *);
#define VF(q, n) (q)->pvt->a[n].pfn((char *)(q) + (q)->pvt->a[n].d)
void func_001F3E00(struct L *p)
{
    struct L *q;
    int unused[4];
    func_001A6150(p);
    func_001D6310(p);
    for (q = p->first; q != p->end; q = q->next) VF(q, 22);
    if (p->pe && p->pe->p60 == p) {
        VF(p, 49);
        RemoveDlEntry__FP2DLPv(&D_003E11AC, p->pe);
    }
}
