typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void *pfn; };
struct SO { char pad[0xC]; struct VTE *vt; char pad1[0x240 - 0x10]; qword pos; };
struct X {
    char pad[0x10]; struct SO *pso; int x14; void *x18; char pad1[0x30 - 0x1C]; int state; float t34;
    char pad2[0x40 - 0x38]; void *x40; float t44; float t48; int x4C; char pad3[0x54 - 0x50]; int x54;
};
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
extern qword D_002A2DD0;
void UpdateBhv(struct X *p);
int FBhvBusyAbove(struct X *p);
int FBhvanimDone(void *p);
int FBhvpathAtEnd(void *p);
int FBhvattackDone(void *p);
int PentryChooseStepguardAttack(struct SO *p, int n, int z);
qword func_001A62E8(void *p);
void func_00155AE0(struct X *p, int k);
void *func_00155A68(struct X *p);

static inline qword Sub(qword a, qword b) { qword d; __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
void func_00155D90(struct X *p)
{
    UpdateBhv(p);
    while (!FBhvBusyAbove(p)) {
        int s = p->state;
        switch (s) {
        case 0:
            if (FBhvanimDone(p->x18)) {
                struct SO *pso;
                qword d = Sub(func_001A62E8(p->x40), p->pso->pos);
                pso = p->pso;
                ((void (*)(void *, qword))pso->vt[29].pfn)((char *)pso + pso->vt[29].d, d);
                pso = p->pso;
                ((void (*)(void *, qword))pso->vt[35].pfn)((char *)pso + pso->vt[35].d, D_002A2DD0);
                pso = p->pso;
                ((void (*)(void *, qword))pso->vt[36].pfn)((char *)pso + pso->vt[36].d, D_002A2DD0);
                p->t48 = D_002DDED0.t;
                func_00155AE0(p, 1);
            }
            break;
        case 2:
            if (FBhvpathAtEnd(p->x18))
                func_00155AE0(p, 1);
            break;
        case 1:
            if (2.5f < D_002DDED0.t - p->t48 && 0.1f < D_002DDED0.t - p->t34) {
                if ((p->x54 = PentryChooseStepguardAttack(p->pso, p->x4C, 0)) != 0)
                    func_00155AE0(p, 3);
            }
            break;
        case 3:
            p->t44 = D_002DDED0.t;
            if (FBhvattackDone(p->x18)) {
                int k = 1;
                void *n = func_00155A68(p);
                if (n != p->x40) {
                    p->x40 = n;
                    k = 2;
                }
                func_00155AE0(p, k);
            }
            break;
        }
        if (s == p->state) return;
    }
}
