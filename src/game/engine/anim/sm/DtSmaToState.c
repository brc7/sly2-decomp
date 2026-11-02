struct V3 { float x, y, z; float w; };
struct T { char pad[0x74]; float f74; float f78; char pad2[0x80 - 0x7C]; struct V3 v80; float f90; float f94; };
struct A { struct T *pt; int n4; int n8; int nC; };
struct Q { char pad[0x14]; struct T *pt; char pad2[0x1C - 0x18]; float f1C; char pad3[0x40 - 0x20]; float f40; };
struct P { char pad[0x24]; void *p24; char pad2[0x30 - 0x28]; struct Q *pq; int n34; char pad3[0x40 - 0x38]; struct A *pa; char pad4[0x48 - 0x44]; int n48; };
extern char D_002DE578[];
int FSmaRetracted(struct P *);
int func_001F0438(void *, int);
struct A *PsmtChooseTowardGoal(void *, int, int, int, int);
float GGetDifficulty(void *);
static inline float Eval(struct V3 *pv, float t)
{
    return pv->x + t * (pv->y + t * pv->z);
}
/** @sly2-annot addr=0x1F11A8
 * Purpose: Estimated time to reach a state: remaining time of the current segment plus durations along the
 *   path (quadratic x + t(y + tz) fits)
 * Info: proposed name DtSmaToState (T1); TU sm
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; sources: semantic_tu_docs/sm
 * See: docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
float DtSmaToState(struct P *p, int k)
{
    float r = 0.0f;
    int end, cur;
    struct A *pa;
    if (FSmaRetracted(p)) return r;
    end = func_001F0438(p->p24, k);
    if (!p->pq) {
        cur = p->n34;
    } else {
        struct Q *pq = p->pq;
        if (!(p->pa->nC & 2)) {
            float d = pq->f40;
            float x;
            if (d < 0.0f) x = -pq->f1C;
            else if (0.0f < d) x = pq->pt->f74 - pq->f1C;
            else return r;
            r = x / d;
        }
        if (p->pa->n4 != p->n34) cur = p->pa->n4; else cur = p->pa->n8;
    }
    while (cur != end && (pa = PsmtChooseTowardGoal(p->p24, p->n48, cur, end, 1)) != 0) {
        if (pa->pt) {
            float t = GGetDifficulty(D_002DE578);
            float v = Eval(&pa->pt->v80, t);
            float mn = pa->pt->f90;
            float mx = pa->pt->f94;
            if (v < mn) v = mn;
            else if (mx < v) v = mx;
            r += pa->pt->f74 / (pa->pt->f78 * v);
        }
        if (pa->n4 != cur) cur = pa->n4;
        else cur = pa->n8;
    }
    return r;
}
