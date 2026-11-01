typedef int qword __attribute__((mode(TI)));
typedef struct { float x, y, z, w; } __attribute__((aligned(16))) VEC;
struct S { char pad[0x1A8]; float g; char p1[0x1714]; int n; char p2[4]; int st; };
extern char D_002BF4C0[];
void ChooseJtAttackTarget(struct S *p, int a, void *pv, int *pn, VEC *pq, float g, float h, float k);
void func_001384C0(void *p);
void SetStepRadTargetVec(struct S *p, qword q);
void FixStepAngularVelocity(struct S *p);
static __inline__ float GClamp(float g, float lo, float hi)
{
    if (g < lo) return lo;
    if (g > hi) return hi;
    return g;
}
void ChooseJtSweepTarget(struct S *p, VEC *pvOut, void *pv)
{
    VEC v;
    int n;
    float t;
    float k = 1.6f;
    if (p->st == 0) k = 3.15f;
    ChooseJtAttackTarget(p, 2, D_002BF4C0, &n, &v, p->g, 0.15f, k);
    if (n) t = GClamp((v.z - -50.0f) * (1.0f / 110.0f), 0.0f, 1.0f);
    else t = 1.0f;
    pvOut->w = t;
    pvOut->x = 1.0f - t;
    func_001384C0(pv);
    p->n = n;
    if (n && p->st == 0) {
        SetStepRadTargetVec(p, *(qword *)&v);
        FixStepAngularVelocity(p);
    }
}
