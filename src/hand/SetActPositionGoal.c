typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
int func_0011D4D0(float a, float b, float eps);
void SlotUnsettle(void *pv, void *px);
struct X { char pad[0x90]; void *p90; char pad2[0xC0 - 0x94]; qword vC0; };
struct P { int x0; void *x4; struct X *x8; int xC; int pad[4]; qword v20; };
extern qword D_002A2DD0;
static __inline__ int FVecsNear(qword a, qword b)
{
    return func_0011D4D0(GetX(a), GetX(b), 1e-4f) && func_0011D4D0(GetY(a), GetY(b), 1e-4f) &&
           func_0011D4D0(GetZ(a), GetZ(b), 1e-4f);
}
void SetActPositionGoal(struct P *p, qword v)
{
    qword dv;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(dv) : "j"(v), "j"(p->v20));
    p->v20 = v;
    if (p->x8->p90 == p && !FVecsNear(D_002A2DD0, dv)) {
        p->x8->vC0 = v;
        SlotUnsettle(p->x4, p->x8);
    }
}
