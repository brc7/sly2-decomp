typedef int qword __attribute__((mode(TI)));
struct G;
extern struct G D_003DD490;
extern float D_002A66A0;
struct E { qword a; qword b; };
struct O { char pad[0x240]; qword x240; };
struct P { struct O *x0; int x4; int pad[2]; struct E ae[1]; };
int CploFindSwObjectsByClass(struct G *g, int k, int n, int m, int c, void **apv);
float func_0019F7D0(struct P *p, struct E *e, int c, void **apv);
void func_002333B4(void *p, int v, int cb);
qword func_001A62E8(void *p);
void func_0019FAF8(struct P *p, struct E *pe)
{
    void *apv[16];
    int c, i;
    float best;
    struct E *e;
    c = CploFindSwObjectsByClass(&D_003DD490, 0x505, 0x33, 0, 0x10, apv);
    best = D_002A66A0;
    for (i = 0; i < p->x4; i++) {
        float g;
        e = &p->ae[i];
        g = func_0019F7D0(p, e, c, apv);
        if (g < best) {
            best = g;
            *pe = *e;
            if (g == 0.0f)
                return;
        }
    }
    if (best == D_002A66A0) {
        qword r;
        func_002333B4(pe, 0, 0x20);
        __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(func_001A62E8(p->x0)), "j"(p->x0->x240));
        pe->b = r;
    }
}
