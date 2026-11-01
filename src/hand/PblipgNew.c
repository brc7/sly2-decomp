struct VTE { short d; short pad; void (*pfn)(); };
struct O { int f0; int f4; int f8; struct VTE *vt; char pad[0x460 - 0x10]; };
struct G { char pad[0x4070]; int pool4070; char pad2[0x407c - 0x4074]; int f407c; int f4080; int f4084; };
extern struct VTE D_002CB5D8[];
struct O *PvAllocSlotheapClear(void *);
void func_002333B4(void *, int, int);
void *PdlFromSwCid(struct G *, int);
void func_0011A070(void *, struct O *);
struct O *PblipgNew(struct G *pg) {
    struct O *po;
    int i;
    if (!pg->f407c && pg->f4084) return 0;
    po = PvAllocSlotheapClear(&pg->pool4070);
    func_002333B4(po, 0, 0x460);
    po->f0 = 0;
    po->f4 = 0;
    po->vt = D_002CB5D8;
    i = 31;
    while (--i != -1) ;   /* empty array-element ctor loop (31 elements) from inlined C++ ctor */
    func_0011A070(PdlFromSwCid(pg, 0x1c), po);
    ((void (*)(void *))po->vt[7].pfn)((char *)po + po->vt[7].d);
    ((void (*)(void *))po->vt[11].pfn)((char *)po + po->vt[11].d);
    ((void (*)(void *))po->vt[15].pfn)((char *)po + po->vt[15].d);
    return po;
}
