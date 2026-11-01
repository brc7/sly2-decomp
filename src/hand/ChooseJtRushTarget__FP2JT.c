typedef struct { float x, y, z, w; } __attribute__((aligned(16))) VEC;
struct T { char pad[0x18]; float g18; float g1c; };
struct S { char pad[0x150]; struct T *pt; char p1[0x54]; float g; char p2[0x1714]; int n; };
extern char D_002BF4D0[];

void ChooseJtAttackTarget(struct S *p, float g, int a, void *pv, int *pn, VEC *pq, float h, float k);

float func_002458F0(float y, float x);
void func_00204660(struct S *p, float g);
void FixStepAngularVelocity(struct S *p);
void ChooseJtRushTarget__FP2JT(struct S *p)
{
    VEC v;
    int n;
    float g;
    if (p->pt && 0.5f < p->pt->g1c) g = p->pt->g18;
    else g = p->g;
    ChooseJtAttackTarget(p, g, 8, D_002BF4D0, &n, &v, 0.25f, 1.0f);
    p->n = n;
    if (n)
        g = func_002458F0(v.y, v.x);
    func_00204660(p, g);
    FixStepAngularVelocity(p);
}
