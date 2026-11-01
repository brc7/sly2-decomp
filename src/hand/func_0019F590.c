typedef int qword __attribute__((mode(TI)));
struct P;
typedef struct { short d; short pad; void (*pfn)(void *, ...); } VT;
typedef struct E { int x0; int x4; int x8; int xc; qword x10; } E;
struct S58 { char pad[0x70]; qword x70; };
struct SE8 { char pad[0x80]; qword x80; };
struct P {
    char pad0[0xC]; VT *vt; char pad1[0x58-0x10]; struct S58 *x58; char pad2[0xE8-0x5C]; struct SE8 *xe8;
    char pad3[0x1B0-0xEC]; char x1b0[0x240-0x1B0]; qword x240; char pad4[0x2C0-0x250]; char x2c0[0x490-0x2C0];
    qword x490; qword x4a0; char pad5[0x9E0-0x4B0]; char x9e0[4]; int x9e4;
};
struct G { char pad[0x3E60]; int x3e60; };
extern struct G D_003DD490;
struct D { char pad[0x60]; qword x60; };
extern struct D D_002DDF40;
extern float D_002BF940;
void func_0019FAF8(void *p, E *e);
struct P *PpoHero(struct G *g);
qword func_0019F3F8(struct P *p, E *e, float g);
float func_00196928(qword a, qword b, float c, float d, float e, float f);
void SetStepRadTargetVec(struct P *p, qword q);
void func_0014D8C0(struct D *d);
void func_00195A00(void *p, int a, int b, int c);
int FIsBasicDerivedFrom__FP5BASIC3CID(int a, int b);
void func_00148300(struct G *g, int a);
void func_0019B110(void *p, int a);
void func_0015E1D8(int a);
static __inline__ qword VSub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
void func_0019F590(struct P *p, E *q)
{
    E e;
    if (!q) {
        func_0019FAF8(p->x9e0, &e);
        q = &e;
        if (p == PpoHero(&D_003DD490)) {
            qword r = func_0019F3F8(p, &e, 0.0f);
            float g = func_00196928(VSub(r, p->x58->x70), p->xe8->x80, 1000.0f, -1000.0f, 1.0f, -1.0f);
            if (D_002BF940 < g) {
                int save = p->x9e4;
                ((void (*)(void *, qword))p->vt[29].pfn)((char *)p + p->vt[29].d, VSub(r, p->x240));
                p->x9e4 = save;
                SetStepRadTargetVec(p, D_002DDF40.x60);
                ((void (*)(void *, void *))p->vt[30].pfn)((char *)p + p->vt[30].d, p->x1b0);
                func_0014D8C0(&D_002DDF40);
                func_00195A00(p->x2c0, 0xB, 0, 0);
                if (e.x4) {
                    if (FIsBasicDerivedFrom__FP5BASIC3CID(e.x0, 0x13) || FIsBasicDerivedFrom__FP5BASIC3CID(e.x0, 0xB) || FIsBasicDerivedFrom__FP5BASIC3CID(e.x0, 0xD) ||
                        FIsBasicDerivedFrom__FP5BASIC3CID(e.x0, 0x27) || FIsBasicDerivedFrom__FP5BASIC3CID(e.x0, 0xC) || FIsBasicDerivedFrom__FP5BASIC3CID(e.x0, 0x22) ||
                        FIsBasicDerivedFrom__FP5BASIC3CID(e.x0, 0x26)) {
                        func_00148300(&D_003DD490, e.x0);
                        func_0019B110(p->x2c0, e.x0);
                    }
                }
                func_0015E1D8(D_003DD490.x3e60);
                return;
            }
        }
    }
    p->x490 = *(qword *)q;
    p->x4a0 = q->x10;
    func_00195A00(p->x2c0, 0xA, 0, 0);
}
