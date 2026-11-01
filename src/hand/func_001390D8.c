struct VTE { short d; short pad; void (*pfn)(); };
struct O { int f0; int f4; int f8; struct VTE *vt; char pad[0x460 - 0x10]; };
struct G { char pad[0x4070]; int pool4070; char pad2[0x407c - 0x4074]; int f407c; int f4080; int f4084; };
extern struct VTE D_002CB5D8[];
struct O *func_0011CCC0(void *);
void func_002333B4(void *, int, int);
void *func_0015E3A8(struct G *, int);
void func_0011A070(void *, struct O *);
/** @sly2-annot addr=0x1390D8
 * Purpose: Allocate a BLIPG from pool g_sw+0x4070 (0x460 bytes, vptr 0x2CB5D8), add to the cid 0x1C list,
 *   call vt7 Init, vt11 AddHierarchy, vt15 PostLoad
 * Info: proposed name PblipgNew (T1); TU blip
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#blip-0x138f08-0x13a9e8
 * @end-sly2-annot */
struct O *func_001390D8(struct G *pg) {
    struct O *po;
    int i;
    if (!pg->f407c && pg->f4084) return 0;
    po = func_0011CCC0(&pg->pool4070);
    func_002333B4(po, 0, 0x460);
    po->f0 = 0;
    po->f4 = 0;
    po->vt = D_002CB5D8;
    i = 31;
    while (--i != -1) ;   /* empty array-element ctor loop (31 elements) from inlined C++ ctor */
    func_0011A070(func_0015E3A8(pg, 0x1c), po);
    ((void (*)(void *))po->vt[7].pfn)((char *)po + po->vt[7].d);
    ((void (*)(void *))po->vt[11].pfn)((char *)po + po->vt[11].d);
    ((void (*)(void *))po->vt[15].pfn)((char *)po + po->vt[15].d);
    return po;
}
