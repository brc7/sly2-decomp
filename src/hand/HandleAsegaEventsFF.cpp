struct E { float t; int k; int x8; int xC; };
struct Q { char pad[0xA0]; int c; struct E *ae; };
struct B { int pad[3]; virtual void Begin(); virtual void End(); };
struct P : B { int x10; struct Q *pq; int x18; float g1C; float g20; char pad2[0x40 - 0x24]; float g40;
    char pad3[0x50 - 0x44]; int i50; };
struct G { P *p; G(P *pp) : p(pp) { p->Begin(); } ~G() { p->End(); } };
extern "C" {
void HandleAsegaEvent(struct P *p, struct E *pe);
int FAsegaRetracted(struct P *p);
}
extern "C" void HandleAsegaEventsFF(struct P *p)
{
    if (p->pq->c) {
    G g(p);
    p->g20 = p->g1C;
    while (!FAsegaRetracted(p)) {
        struct Q *pq = p->pq;
        int i = p->i50;
        float t = p->g20;
        struct E *pe;
        if (i < pq->c && pq->ae[i].t <= t && (pe = &pq->ae[i], 1) && 0.0f <= p->g40) {
            p->i50 = i + 1;
        } else if (p->i50 > 0 && t <= p->pq->ae[p->i50 - 1].t && p->g40 < 0.0f) {
            p->i50 = p->i50 - 1;
            pe = &p->pq->ae[p->i50];
        } else {
            p->g1C = t;
            break;
        }
        switch (pe->k) {
        case 7: case 8: case 11: case 13: case 14: case 15: case 16:
        case 21: case 22: case 23: case 24:
            HandleAsegaEvent(p, pe);
            break;
        }
    }
    }
}
