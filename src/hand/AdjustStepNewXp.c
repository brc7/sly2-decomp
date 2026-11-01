/* Prepare a new contact: reset its friction/response and set the limits for kind 14; for the
   side-facing kinds add a horizontal response contact (kind 10) through func_001A7C18. */
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
struct ENDPOINT { void *shape, *owner, *object, *lo; qword pos; qword local; int pad[4]; };
struct XP { struct ENDPOINT ae[2]; U normal; float distance, friction, response; int kind; float lower, upper; int tag; struct XP *next; };
struct STATE { char pad[0xD0]; float mass; };
struct P { char padE8[0xE8]; struct STATE *state; char padEC[0x258 - 0xEC]; void *mesh; };
extern char D_003DD490[];
extern qword D_002A2DA0;
extern float D_002C3C60[], D_002C3C90;
void func_001F4FB0(void);
void func_001A7C18(void *pw, void *a, void *b, qword pos, qword dir, int k, int x, int y, struct XP **out, float g);

/* The endpoint positions are passed by address (like C++ const VECTOR & operands): the address
   then comes out as offset + x, and the second read of the other position is a reload. */
static inline qword Sub(const qword *pa, const qword *pb)
{
    qword a = *pa, b = *pb, r;
    __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
static inline qword Scale(qword a, float g)
{
    qword k;
    U u;
    __asm__("qmtc2.ni %1,%0" : "=j"(k) : "r"(g));
    u.q = k;
    __asm__("vmulx.xyzw %0,%1,%2x" : "=j"(a) : "j"(a), "j"(k));
    return a;
}
static inline float Dot(qword a, qword b)
{
    qword d, one;
    float g;
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=&j"(d), "=&j"(one) : "j"(a), "j"(b));
    __asm__("qmfc2.ni %0,%1" : "=r"(g) : "j"(d));
    return g;
}
static inline void Normal(U *v, qword fallback)
{
    qword a = v->q;
    register qword d __asm__("vf1"); /* the one pin: the length/result lives in vf1 (as in func_0017F1D0) */
    qword one;
    float len;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=&j"(d), "=&j"(one) : "j"(a));
    __asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0,%1" : "=r"(len) : "j"(d));
    if (len < 0.0001f)
        d = fallback;
    else
        __asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q" : "=j"(d) : "j"(d), "j"(a));
    v->q = d;
}
static inline qword AddScaled(const qword *pa, qword b, float g)
{
    qword a = *pa, k;
    U u;
    __asm__("qmtc2.ni %1,%0" : "=j"(k) : "r"(g));
    u.q = k;
    __asm__("vadda.xyzw $ACC,%0,$vf0\n\tvmaddx.xyzw %0,%1,%2x" : "=j"(a) : "j"(b), "j"(k), "0"(a));
    return a;
}

void AdjustStepNewXp(struct P *p, struct XP *x, int side)
{
    U dir, diff, point;
    float d;
    float sign;
    int kind;
    func_001F4FB0();
    kind = x->kind;
    x->friction = 0.0f;
    x->response = 0.0f;
    if (kind == 14) {
        if (x->normal.f[2] > 0.7f) {
            x->friction = D_002C3C90;
            x->lower = p->state->mass * -4500.0f;
            x->upper = p->state->mass * 9000.0f;
        } else
            x->upper = p->state->mass * 9000.0f;
        return;
    }
    if (kind != 3) {
        if (side != 1)
            return;
        if (kind != side && kind != 4 && kind != 5 && kind != 6)
            return;
    }
    sign = D_002C3C60[side];
    if (sign * x->normal.f[2] > 0.0f) {
        /* horizontal direction of the (side-signed) normal */
        dir.q = Scale(x->normal.q, sign);
        dir.f[2] = 0.0f;
        Normal(&dir, D_002A2DA0);
        /* project this endpoint onto the other along that direction */
        diff.q = Sub(&x->ae[side].pos, &x->ae[1 - side].pos);
        d = Dot(diff.q, dir.q);
        point.q = AddScaled(&x->ae[1 - side].pos, dir.q, d);
        func_001A7C18(D_003DD490, p->mesh, x->ae[1 - side].shape, point.q, dir.q, 10, 0, 0, &x->next, d);
    }
}
