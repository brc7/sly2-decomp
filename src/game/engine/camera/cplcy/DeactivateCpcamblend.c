struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a[3]; struct VTE *vt; };
struct B { int pad[3]; int nC; int pad10; struct O *po; };
struct A { int pad[4]; struct B *pb; };
extern char D_002DDF40[];
void DeactivateCplcy(struct A *a, int n, struct B *b);
void CutCm__FP2CM(void *p);
/** @sly2-annot addr=0x151698
 * Purpose: CPLCY::Deactivate; CutCm if fCutOnLeave(+0xC); pevent vt2 (end event); pcammove=0
 * Info: proposed name DeactivateCpcamblend (T1); TU cplcy
 * Confidence: mid (draft) [T1 0.75] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void DeactivateCpcamblend(struct A *a, int n, struct B *b)
{
    struct O *o;
    struct VTE *e;
    DeactivateCplcy(a, n, b);
    if (b->nC)
        CutCm__FP2CM(D_002DDF40);
    o = a->pb->po;
    e = &o->vt[2];
    e->pfn((char *)o + e->d);
    a->pb = 0;
}
