typedef int qword __attribute__((mode(TI)));
typedef struct { float x, y, z, w; } __attribute__((aligned(16))) VEC;
struct S { char pad[0x1A8]; float g; char p1[0x1714]; int n; };
extern char D_002BF4E0[];
void ChooseJtAttackTarget(struct S *p, int a, void *pv, int *pn, VEC *pq, float g, float h, float k);
void SetStepRadTargetVec(struct S *p, qword q);
void FixStepAngularVelocity(struct S *p);
void ChooseJtSmashTarget__FP2JT(struct S *p)
{
    VEC v;
    int n;
    ChooseJtAttackTarget(p, 4, D_002BF4E0, &n, &v, p->g, 0.25f, 3.15f);
    p->n = n;
    if (n) {
        SetStepRadTargetVec(p, *(qword *)&v);
        FixStepAngularVelocity(p);
    }
}
