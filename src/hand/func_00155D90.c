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
void func_001EEF98(struct X *p);
int func_001EF0A8(struct X *p);
int func_00174FB8(void *p);
int func_00178D00(void *p);
int func_00175748(void *p);
int func_001758A0(struct SO *p, int n, int z);
qword func_001A62E8(void *p);
void func_00155AE0(struct X *p, int k);
void *func_00155A68(struct X *p);

static inline qword Sub(qword a, qword b) { qword d; __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
/** @sly2-annot addr=0x155D90
 * Purpose: Run/hide state machine for the fleeing guard
 * Info: proposed name UpdateBhv181 (T2); class BHV181 slot10; TU tu_1553F0
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_1553f0-0x1553f0-0x1564e8
 * @end-sly2-annot */
void func_00155D90(struct X *p)
{
    func_001EEF98(p);
    while (!func_001EF0A8(p)) {
        int s = p->state;
        switch (s) {
        case 0:
            if (func_00174FB8(p->x18)) {
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
            if (func_00178D00(p->x18))
                func_00155AE0(p, 1);
            break;
        case 1:
            if (2.5f < D_002DDED0.t - p->t48 && 0.1f < D_002DDED0.t - p->t34) {
                if ((p->x54 = func_001758A0(p->pso, p->x4C, 0)) != 0)
                    func_00155AE0(p, 3);
            }
            break;
        case 3:
            p->t44 = D_002DDED0.t;
            if (func_00175748(p->x18)) {
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
