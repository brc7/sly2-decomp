typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
typedef struct { int a, b; } S8;
typedef struct {
    struct { char pad[8]; void *x8; char pad2[0x34]; } ab[2];
    qword x80;
    char pad90[4];
    float x94;
    float x98;
} OBJ;
extern char D_003DD490[];
extern S8 D_002A7840;
void func_001F4FB0(void *, OBJ *, int);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
void EnsureSwCallback(void *, void *, S8, void *);
static inline float GetZ(qword q) { float f; __asm__("pextuw %0, $0, %1" : "=r"(f) : "r"(q)); return f; }
void AdjustRobottankNewXp(void *a, OBJ *p, int n)
{
    void *e;
    qword v;
    func_001F4FB0(a, p, n);
    e = p->ab[1 - n].x8;
    if (n == 0) {
        v = p->x80;
    } else {
        U s;
        __asm__("qmtc2.ni %1, %0" : "=j"(s.q) : "r"(-1.0f));
        __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(v) : "j"(p->x80), "j"(s.q));
    }
    if (FIsBasicDerivedFrom__FP5BASIC3CID(e, 0x4D) || GetZ(v) < 0.7f) {
        p->x94 = 0.0f;
        p->x98 = 0.1f;
    } else {
        p->x94 = 0.5f;
        p->x98 = 0.1f;
    }
    if (FIsBasicDerivedFrom__FP5BASIC3CID(e, 0x6C))
        EnsureSwCallback(D_003DD490, a, D_002A7840, e);
}
