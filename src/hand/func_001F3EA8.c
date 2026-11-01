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
int FFindDlEntry__FP2DLPv(struct DL *, void *);
void AppendDlEntry__FP2DLPv(struct DL *, void *);
#define VF(q, n) (q)->pvt->a[n].pfn((char *)(q) + (q)->pvt->a[n].d)
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
        if (!FFindDlEntry__FP2DLPv(&D_003E11AC, p->pe)) AppendDlEntry__FP2DLPv(&D_003E11AC, p->pe);
        VF(p, 49);
    }
}
