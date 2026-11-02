struct VTE { short d; short pad; void (*pf)(); };
struct O { int x0; int x4; int x8; struct VTE *vt; char p1[0x80 - 0x10]; char x80[0xD0]; int x150; };
struct G { char pad[0x403C]; char pool[0x30]; struct O *x406C; };
extern struct VTE D_002D9978[];
extern struct VTE D_002D77D8[];
void *PvAllocSlotheapClear(void *pool);
void *func_002333B4(void *p, int c, int cb);
void *func_00167310(void *p);
void *PdlFromSwCid(struct G *p, int n);
void func_0011A070(void *pl, void *pv);
#define VCALL0(o, n) ((void (*)(void *))(o)->vt[n].pf)((char *)(o) + (o)->vt[n].d)
/** @sly2-annot addr=0x1CDCD0
 * Purpose: Get/allocate a RIPG: if !fNew return cached default SW+0x406C, else take one from SW RIPG pool
 *   (+0x403C), zero it, construct (CRef), add to world
 * Info: proposed name PripgNew (T1); TU rip
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/particles-effects.md#5-source-files, docs/engine/particles-effects.md#6-key-functions,
 *   docs/engine/rendering.md#12-source-files
 * @end-sly2-annot */
struct O *PripgNew(struct G *p, int a)
{
    struct O *po;
    if (!a && p->x406C) return p->x406C;
    po = PvAllocSlotheapClear(p->pool);
    if (!po) return 0;
    func_002333B4(po, 0, 0x1B0);
    po->x0 = 0;
    po->x4 = 0;
    po->vt = D_002D9978;
    func_00167310(po->x80);
    po->x150 = a;
    po->vt = D_002D77D8;
    func_0011A070(PdlFromSwCid(p, 0x57), po);
    VCALL0(po, 7);
    VCALL0(po, 11);
    VCALL0(po, 15);
    if (!a) p->x406C = po;
    return po;
}
