// MATCH (Claude agent laneB9, 2026-10-23): the t copy pinned to $f1 (one commented register pin) declared inside an `if (i < pq->c)` block, take-branch ends with continue (reorg puts the mov.s in the bc1t delay slot).
// fresh1 (Claude agent, 2026-10-23): aligned 16 -> 15: pq->ae[i] indexed in the compare, then (pe = &pq->ae[i], 1) as in func_00131100 (fixes addu order). Left: target t in f2 plus a mov.s f1,f2 copy used only by c.eq.s (bc1t delay slot).
struct E { float t; int k; int x8; int xC; };
struct Q { char pad[0xA0]; int c; struct E *ae; };
struct B { int pad[3]; virtual void Begin(); virtual void End(); };
struct P : B { int x10; struct Q *pq; int x18; float g1C; float g20; char pad2[0x40 - 0x24]; float g40;
    char pad3[0x50 - 0x44]; int i50; };
struct G { P *p; G(P *pp) : p(pp) { p->Begin(); } ~G() { p->End(); } };
extern "C" {
void func_00131290(struct P *p, struct E *pe);
int func_00131968(struct P *p);
}
static inline int FGe(float a, float b) { return b < a || a == b; }
static inline int FGe2(float a, float b) { return b < a || (a == b); }
static inline int FEq(float a, float b) { return a == b; }
static inline int FB(float a, float b, float d) { return a < b || (b == a && 0.0f < d); }
static inline int FB2(float b, float a, float d) { return a < b || (b == a && 0.0f < d); }
static inline int FC(float a, float b, float d) { return a < b || (a == b && d < 0.0f); }
/** @sly2-annot addr=0x130F88
 * Purpose: Dispatches events crossed during the frame (calls HandleAsegaEvent)
 * Info: proposed name HandleAsegaEvents (T1); TU asega
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#asega-0x1308a8-0x131f70
 * @end-sly2-annot */
extern "C" void func_00130F88(struct P *p)
{
    if (p->pq->c) {
    G g(p);
    p->g20 = p->g1C;
    while (!func_00131968(p)) {
        struct Q *pq = p->pq;
        int i = p->i50;
        float t = p->g20;
        struct E *pe;
        if (i < pq->c) {
            register float tc __asm__("$f1") = t; /* register pin: t copy in f1 for the == test */
            if ((pq->ae[i].t < t || (tc == pq->ae[i].t && 0.0f < p->g40)) && (pe = &pq->ae[i], 1)) {
                p->i50 = i + 1;
                func_00131290(p, pe);
                continue;
            }
        }
        if (p->i50 > 0 && (t < p->pq->ae[p->i50 - 1].t || (t == p->pq->ae[p->i50 - 1].t && p->g40 < 0.0f))) {
            p->i50 = p->i50 - 1;
            func_00131290(p, &p->pq->ae[p->i50]);
        } else {
            p->g1C = t;
            break;
        }
    }
    }
}
