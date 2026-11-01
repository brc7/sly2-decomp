typedef int qword __attribute__((mode(TI)));
extern "C" {
void func_001AE478(void *p);
void func_001AE4A0(void *p, int n);
void func_0020FAA0(void *g, qword a, qword b, void *p1, void *p2, int *pi, int *pj);
struct HIT;
int func_0014CC58(int z, qword a, qword b, int i, int j, HIT *phit);
void EvaluateBezierPos(float tMax, float t, float s, qword a, qword va, qword b, qword vb, qword *ppos, int m, int n);
extern char D_003DD490[];
}
class FRAME { public: FRAME(); ~FRAME(); };
struct HIT { qword pos; char pad[0x34]; float x44; char pad2[0x18]; };
static inline float Dist(qword a, qword b)
{
    qword d, t;
    float r;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z\n\tvsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "+j"(d), "=&j"(t));
    __asm__("qmfc2.ni %0, %1" : "=r"(r) : "j"(d));
    return r;
}
extern "C" int func_0019AE00(void *unused, void *p1, void *p2, float tMax, qword a, qword va, qword b, qword vb, qword *pc);
int func_0019AE00(void *unused, void *p1, void *p2, float tMax, qword a, qword va, qword b, qword vb, qword *pc)
{
    struct V { float x, y, z; } __attribute__((aligned(16))) pos;
    HIT hit;
    FRAME frame;
    int i, j;
    qword mn, mx, prev;
    float t;
    if (pc) {
        __asm__("vmax.xyz %0, %2, %3\n\tvmini.xyz %1, %2, %3" : "=&j"(mx), "=j"(mn) : "j"(b), "j"(*pc));
        __asm__("vmini.xyz %1, %2, %1\n\tvmax.xyz %0, %2, %0" : "+j"(mx), "+j"(mn) : "j"(a));
    } else {
        mn = a;
        mx = b;
    }
    func_0020FAA0(D_003DD490, mn, mx, p1, p2, &i, &j);
    for (t = 0.0f; ; t = t + 0.1f < tMax ? t + 0.1f : tMax) {
        EvaluateBezierPos(tMax, t, 1.0f, a, va, b, vb, (qword *)&pos, 0, 0);
        if (t != 0.0f && func_0014CC58(0, prev, *(qword *)&pos, i, j, &hit)) {
            if (100.0f < Dist(hit.pos, b)) return 1;
        }
        prev = *(qword *)&pos;
        if (t == tMax) break;
    }
    if (pc && func_0014CC58(0, b, *pc, i, j, &hit) && hit.x44 != 1.0f && 50.0f < Dist(hit.pos, *pc))
        return 1;
    return 0;
}
