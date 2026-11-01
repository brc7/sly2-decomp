struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { VTE a[3]; };
struct P { char pad[0xC]; VT *pvt; char pad2[0x30 - 0x10]; void *p30; char pad3[0x44 - 0x34]; float g; };
extern "C" int FSmaRetracted(P *);
extern "C" void func_00131A00(void *, float);
struct Lock {
    P *p;
    Lock(P *pp) { p = pp; p->pvt->a[1].pfn((char *)p + p->pvt->a[1].d); }
    ~Lock() { p->pvt->a[2].pfn((char *)p + p->pvt->a[2].d); }
};
/** @sly2-annot addr=0x1F0850
 * Purpose: Stores +0x44; SetAsegaSpeed on the running ASEGA
 * Info: proposed name SetSmaSvt (T1); TU sm
 * Notes: names_v2 v1 proposed SeekSma__FP3SMA3OID (0.001)
 * Confidence: high [T1 0.85] - evidence: semantic review; static naming pass; sources: semantic_tu_docs/sm
 * See: docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
extern "C" void SetSmaSvt(P *p, float g)
{
    if (!FSmaRetracted(p)) {
        Lock l(p);
        p->g = g;
        if (p->p30) func_00131A00(p->p30, g);
    }
}
