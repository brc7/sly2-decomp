// WakeSoWater (0x15A2C0): wake sound, ripple and splash for an SO moving through WATER.
// (Lilly's agent, T1683) MATCH, no register pins. Built from the lilly1/v3 drafts (Claude agents, T1679 / lane E20).
//
// Sly 2 signature (SO *, WATER *, qword pos, qword v, float dt, float gFrac, float gScale) despite the Sly 1 mangled name.
// The splash is func_0015BE28(pos, dir, cParticle, EMITP, EMITV, &e, 0, 0) with the EMITP (0x40) and EMITV (0x70) blocks
// passed BY VALUE (EABI: a3 / t0 point at the caller's copies). They are aggregate initializers: g++ builds each one
// in a memset stack temp and block-copies it, which is the target's memset + stores + lq/sq copy. What matched is the
// unions in the two types (as in func_0015CAD8): store_constructor emits a (clobber (mem:BLK)) for every union
// initializer, and those clobbers are scheduler barriers.
// - EMITV: G4V at +0x00, a 4-float union at +0x40 and Sly 1's CLQ at +0x60 give the target's FPU numbers for
//   5.0 / 0.39 / 1.047 / -1 (-1 reuses 0.39's register) and the store order.
// - EMITP: a CLQ at +0x18 ({8, 12, 1}; Sly 1's EMITRIP has a clqScale) puts the +0x10 store before the +0xC store.
//   Any union holding +0x18/+0x1C matches; field names other than the Sly 1 types are unknown.
// - E is 0x14 bytes (memset sizeof); the splash position (v raised 30 in z) is its own stack local.
typedef int qword __attribute__((mode(TI)));

// Sly 1 util.h CLQ: quadratic g0 + x*(g1 + x*g2).
union CLQ {
    struct { float g0, g1, g2, gUnused; };
    float ag[4];
};

// 16-byte float views; the field names are unknown (same type as in func_0015CAD8).
union G4V { struct { float g0, g4, g8, gC; }; float ag[4]; };

struct EMITP {
    int x0;
    int x4;
    int x8, xC, x10; // oids of WATER+0x178, WATER+0x17C and the WATER itself
    int x14;
    CLQ clq18;
    int x28, x2C;
    qword x30; // some 16-byte member: the target copies EMITP with lq/sq, so the type is 16-aligned
};

struct EMITV {
    G4V g0;
    qword v10;
    qword v20;
    int x30;
    int x34;
    float x38, x3C;
    G4V g40;
    int x50[4];
    CLQ clq60;
};

typedef char check_emitp[(sizeof(EMITP) == 0x40) ? 1 : -1];
typedef char check_emitv[(sizeof(EMITV) == 0x70) ? 1 : -1];

struct VTE { short d; short pad; qword (*pfn)(void *); };
struct VT { VTE a[64]; };
struct XE8 { char pad[0x80]; qword v; };
struct SO { int x0, x4, x8; VT *pvt; char pad10[0xE8 - 0x10]; XE8 *pxE8; };
struct SND { char pad[0x170]; float t; void *psnd; };
struct EVT { short d; short pad; void (*pfn)(void *, float, qword, int); };
struct EVTT { EVT a[4]; };
struct EX { char pad[0x18]; int id; };
struct WATER { char pad[0x18]; int x18; char pad1c[0x178 - 0x1C]; EX *px178; EX *px17C; int x180; };
struct RIP { char pad[0x1C]; EX *pex; float g20; float g24; char pad28[0xEC - 0x28]; EVTT *pvt; };
struct E { int x0, x4; EX *p8; EX *pC; WATER *p10; };
union V4 { qword q; float a[4]; };
struct CLOCK { float t; };

extern "C" {
extern CLOCK D_002DDED0;
extern int D_002BDEB0[];
extern qword D_002A2DC0;
extern char D_003DD490[];
float GRandInRange__Fff(float lo, float hi);
int NRandInRange__Fii(int lo, int hi);
int func_00132B10(SO *p, int n);
void func_001FB1F8(void *p, float g);
void func_001FB098(void *p, float g);
void *func_001FBB90(int id);
void func_001FB2F8(void *p, SO *pso);
void func_001FB4F0(void *p, float a, float b);
void func_00258040(void *pp, void *p);
RIP *func_001CE888(void *p, int k, int x);
void *func_002333B4(void *p, int c, int cb);
void func_0015BE28(qword pos, qword dir, int n, EMITP emitp, EMITV emitv, E *pe, int x, int y);
}

static inline qword Length(qword a)
{
    qword r, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z\n\t"
            "vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q"
        : "=j"(r), "=j"(one) : "j"(a));
    return r;
}
static inline qword Sub(qword a, qword b)
{
    qword r;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
static inline float GClamp(float g, float lo, float hi)
{
    if (g < lo)
        return lo;
    if (hi < g)
        return hi;
    return g;
}
static inline float GetX(qword q)
{
    float f;
    __asm__("mtc1 %1, %0" : "=f"(f) : "r"(q));
    return f;
}

void WakeSoWater(SO *p, WATER *pwater, qword pos, qword v, float dt, float gFrac, float gScale) __asm__("WakeSoWater__FP2SOP5WATERfP6VECTORT3ff");
void WakeSoWater(SO *p, WATER *pwater, qword pos, qword v, float dt, float gFrac, float gScale)
{
    float g = 0.0f;
    VTE *pvte = &p->pvt->a[33];
    float d = GetX(Length(Sub(pos, pvte->pfn((char *)p + pvte->d))));
    int fWake = GRandInRange__Fff(g, 3.0f) < dt;
    SND *psnd;
    float t, u;
    psnd = 0;
    if (func_00132B10(p, 0x32))
        psnd = (SND *)p;
    if (d > 10.0f || fWake) {
        u = (d - 10.0f) * 0.0012658228f;
        t = GClamp(u, g, 1.0f);
        if (psnd && pwater->x180) {
            void **pp = &psnd->psnd;
            g = t * 0.9f + 0.1f;
            if (psnd->psnd) {
                func_001FB1F8(psnd->psnd, g);
            } else {
                void *ps = func_001FBB90(D_002BDEB0[NRandInRange__Fii(0, 3)]);
                func_001FB2F8(ps, (SO *)psnd);
                func_001FB4F0(ps, 100.0f, 1000.0f);
                func_001FB1F8(ps, g);
                func_00258040(pp, ps);
            }
        }
        if (!fWake) {
            u = 1.0f - t * (1.0f - gFrac);
            if (psnd) {
                fWake = u < D_002DDED0.t - psnd->t;
            } else {
                float tc = D_002DDED0.t;
                int c0 = (int)(tc / u);
                int c1 = (int)((tc + dt) / u);
                fWake = c0 != c1;
            }
        }
        if (fWake && pwater->px17C) {
            RIP *prip = func_001CE888(D_003DD490, 7, 0);
            if (prip) {
                EVT *pe = &prip->pvt->a[1];
                pe->pfn((char *)prip + pe->d, gScale, v, 0);
                prip->g20 = gScale;
                prip->g24 = gScale * 0.6f;
                prip->pex = pwater->px17C;
                if (psnd)
                    psnd->t = D_002DDED0.t;
                if (500.0f < d && pwater->px178) {
                    EMITP emitp = { 1, 0, pwater->px178->id, pwater->px17C->id, pwater->x18, 0, {{ 8.0f, 12.0f, 1.0f }} };
                    EMITV emitv = { {{ d * 0.5f, d * 0.75f, 0.0f, 1.0f }}, p->pxE8->v, D_002A2DC0, 0, 0, 1.0f, 5.0f,
                                    {{ 0.39269909f, 1.0471976f }}, { 0 }, {{ 1.0f, 0.0f, -1.0f }} };
                    E e;
                    V4 posSplash;
                    func_002333B4(&e, 0, sizeof(e));
                    posSplash.q = v;
                    posSplash.a[2] += 30.0f;
                    e.p8 = pwater->px178;
                    e.pC = pwater->px17C;
                    e.p10 = pwater;
                    func_0015BE28(posSplash.q, D_002A2DC0, NRandInRange__Fii(4, 6), emitp, emitv, &e, 0, 0);
                }
            }
        }
    } else if (psnd) {
        void **pp = &psnd->psnd;
        if (pp && *pp) {
            func_001FB098(*pp, g);
            func_00258040(pp, 0);
        }
    }
}
