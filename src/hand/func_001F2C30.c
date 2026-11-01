typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[12]; };
struct S8 { int a, b; };
struct E { char pad[0xDC]; void *pDC; };
struct L { char pad[0xC]; struct VT *pvt; char pad1[0x24 - 0x10]; struct L *next; char pad1b[0x40 - 0x28]; int v40; char pad2[0x64 - 0x44]; struct L *first; char pad3[0x70 - 0x68]; struct L *end; char pad4[0xE8 - 0x74]; struct E *pe; };
struct H { char pad[0x2C]; int n2C; };
struct G { char pad[0x45CC]; struct H *ph; };
extern struct G D_003DD490;
extern struct S8 D_002A8270;
void func_001A5800(struct L *);
void func_0022FAA0(struct L *);
void func_001F2B20(struct L *);
qword func_001A62E8(struct L *);
void *func_001C0D70(qword, int);
void func_001F37C8(struct L *, void *);
void func_00149A50(struct G *, struct L *, struct S8, int);
/** @sly2-annot addr=0x1F2C30
 * Purpose: SO vt11: AddLoHierarchy, 0x22FAA0, AddSoParts, set grf 0x14, register region (if SOPHYS region
 *   record), recurse vt11 on children
 * Info: proposed name AddSoHierarchy (T1); class SO slot11; TU so
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/particles-effects.md#trail-0x21bb88-0x21c820,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F2C30(struct L *p)
{
    struct L *q;
    void *pv;
    int unused[4];
    int v;
    func_001A5800(p);
    func_0022FAA0(p);
    func_001F2B20(p);
    v = p->v40;
    if ((v ^ 1) & 1) *(int *)v |= 0x14;
    else p->v40 = v | 0x14;
    pv = p->pe ? p->pe->pDC : 0;
    if (pv) {
        if (D_003DD490.ph && D_003DD490.ph->n2C)
            func_001F37C8(p, func_001C0D70(func_001A62E8(p), 1));
        else
            func_00149A50(&D_003DD490, p, D_002A8270, 0);
    }
    for (q = p->first; q != p->end; q = q->next)
        q->pvt->a[11].pfn((char *)q + q->pvt->a[11].d);
}
