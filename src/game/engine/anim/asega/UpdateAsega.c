struct VTE { short d; short pad; void (*pfn)(); };
struct Q { char pad[0x40]; unsigned int x40; char pad1[0xA0 - 0x44]; int cA0; };
struct L { struct { struct VTE *vt; } *po; int x4; int x8; int xC; struct L *pnext; };
struct P { int pad[3]; struct VTE *vt; int x10; struct Q *pq; int x18; float g1C; float g20; char pad2[0x38 - 0x24]; struct L *pl; int grf;
    float g40; char pad3[0x50 - 0x44]; int i50; };
struct G { struct P *p; };
struct CLOCK { float t; float dt; char pad[0x38]; float g40; };
extern struct CLOCK D_002DDED0;
void HandleAsegaEvents(struct P *p);
int FAsegaRetracted(struct P *p);
int FWrapAsegaTime(struct P *p, float *pt, float *pdt);
void func_00130B18(struct P *p);
static __inline__ unsigned int GrfQ(struct Q *pq) { unsigned int x = pq->x40; return ((x ^ 1) & 1) ? *(unsigned int *)x : x; }
static __inline__ void End(struct G *pg) { struct P *po = pg->p; ((void (*)(void *))po->vt[2].pfn)((char *)po + po->vt[2].d); }
/** @sly2-annot addr=0x130918
 * Purpose: Per-frame advance: time wrap (0x1317D8), event dispatch (0x130F88), retract when finished
 * Info: proposed name UpdateAsega (T1); TU asega
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r1_116918
 * See: docs/engine/animation.md#1-aseg-and-asega-asegcpp-0x12f248-asegacpp-0x1308a8,
 *   docs/reference/functions/animation.md#asega-0x1308a8-0x131f70,
 *   docs/reference/functions/world-sw.md#update-0x226550-0x226ab8
 * @end-sly2-annot */
void UpdateAsega(struct P *p)
{
    if (__builtin_fabsf(p->g40) < 1e-4f)
        return;
    {
        struct G g;
        unsigned int x;
        int f;
        struct L *pl;
        g.p = p;
        __asm__ __volatile__("" : : "r"(&g) : "memory");
        ((void (*)(void *))p->vt[1].pfn)((char *)p + p->vt[1].d);
        if (GrfQ(p->pq) & 0x100)
            p->g1C = p->g1C + p->g40 * D_002DDED0.g40;
        else
            p->g1C = p->g1C + p->g40 * D_002DDED0.dt;
        HandleAsegaEvents(p);
        if (FAsegaRetracted(p))
            { End(&g); return; }
        f = FWrapAsegaTime(p, &p->g1C, &p->g40);
        if (f) {
            if (p->grf & 8)
                p->i50 = p->g40 <= 0.0f ? p->pq->cA0 : 0;
        }
        if (f) {
            ((void (*)(void *, int, struct P *))p->vt[4].pfn)((char *)p + p->vt[4].d, 3, p);
            if (FAsegaRetracted(p))
                { End(&g); return; }
        }
        HandleAsegaEvents(p);
        if (FAsegaRetracted(p))
            { End(&g); return; }
        if (f && !(p->grf & 0x38))
            func_00130B18(p);
        for (pl = p->pl; pl; pl = pl->pnext)
            ((void (*)(void *, int, int, float, float))pl->po->vt[2].pfn)((char *)pl->po + pl->po->vt[2].d, pl->x4, 0, p->g1C, p->g40);
        End(&g);
    }
}
