typedef int qword __attribute__((mode(TI)));
struct VT { struct { short d; short pad; void (*pfn)(void *, qword); } a[40]; };
struct SO;
struct P { char pad[0xC]; struct VT *pvt; char pad1[0x190 - 0x10]; qword v190; char pad2[0x1B0 - 0x1A0]; qword mat[3];
    char pad4[0x40C - 0x1E0]; int f40C; };
struct SO { char pad[0x18]; struct P *p18; };
struct MSG { struct SO *pso; float g; int k; };
void HandlePoMessage(struct P *p, int msgk, struct MSG *pmsg);
qword func_00204E38(struct P *p, struct SO *pso, float g);
static inline qword VecMul(qword v, float g)
{
    qword r;
    union { qword q; float f[4]; } s;
    __asm__("qmtc2.ni %1, %0" : "=j"(s.q) : "r"(g));
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v), "j"(s.q));
    return r;
}
void func_00152250(struct P *p, int msgk, struct MSG *pmsg)
{
    HandlePoMessage(p, msgk, pmsg);
    if (msgk == 9) {
        struct P *pp = pmsg->pso->p18;
        if (pp == p) {
            switch (pmsg->k) {
            case 0x27:
                pp->f40C = 1;
                break;
            case 0x28:
                pp->f40C = 0;
                break;
            case 0x15E: {
                qword v = func_00204E38(pp, pmsg->pso, pmsg->g);
                struct VT *pvt = pp->pvt;
                qword *pm = pp->mat;
                qword r;
                pp->v190 = VecMul(v, -1.0f);
                __asm__("vmulax.xyz ACC, %1, %2x\n\tvmadday.xyz ACC, %3, %2y\n\tvmaddz.xyz %0, %4, %2z" : "=j"(r) : "j"(pm[0]), "j"(v), "j"(pm[1]), "j"(pm[2]));
                ((void (*)(void *, qword))pvt->a[35].pfn)((char *)pp + pvt->a[35].d, r);
                break;
            }
            }
        }
    }
}
