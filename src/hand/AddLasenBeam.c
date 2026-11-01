struct V { float x, y, z; int m; };
struct Q { int n; int pad[5]; struct V v; char pad2[0xA0-0x28]; };
struct E { struct Q q0; struct Q q1; char pad[0x150-0x140]; };
struct S { char pad[0x198]; int c; struct E *ae; };
static inline void SetV(struct V *pv, float x, float y, float z, int m) {
    pv->x = x;
    pv->y = y;
    pv->z = z;
    pv->m = m;
}
void AddLasenBeam(struct S *p, int n0, float x0, float y0, float z0, int m0, int n1, float x1, float y1, float z1, int m1) {
    if (p->c < 0x40) {
        struct Q *pq; struct V *pv;
        pq = &p->ae[p->c].q0;
        pq->n = n0;
        pv = &pq->v; SetV(pv, x0, y0, z0, m0);
        pq = &p->ae[p->c].q1;
        pq->n = n1;
        pv = &pq->v; SetV(pv, x1, y1, z1, m1);
        p->c++;
    }
}
