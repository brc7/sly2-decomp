typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct CM { char pad[0x20]; float g20; };
extern struct CM D_002DDF40;
struct G { char pad[0x3B4C]; void *p3B4C; char pad2[0x4750 - 0x3B50]; float g4750; };
extern struct G D_003DD490;
extern char D_003D4A60[];
struct POLY { float c0, c1, c2; };
struct LM { float gMin, gMax; };
extern struct POLY D_002BD980;
extern float D_002BD990, D_002BD994, D_002BD998, D_002BD99C;
extern char D_002BD9A0[];
extern struct LM D_002BD9B0;
extern char D_002BBB60[], D_002BBB68[];
struct CRef { int m_tagk; int m_n; };
struct SND { char pad[0x4E]; short c4E; };
struct JOY { char pad[0x103C]; float g103C; float g1040; char pad2[0x1074 - 0x1044]; float g1074; };
struct X {
    char pad0[0x10]; float g10; float g14; float g18; float g1C; float g20; float g24; int pad28; float g2C; float g30;
    struct SND *p34; int pad38; float g3C; float g40; float g44; float g48;
};
void func_0014FD40(struct X *p, void *pv, struct JOY *pjoy);
float GSmooth__FfffP3SMPPf(float gCur, float gTarget, float dt, void *psmp, float *pdgNext);
float func_0011D948(void *p, float g);
void func_001FB2C0(struct SND *p, float g);
void func_001FB1F8(struct SND *p, float g);
void func_001FB060(struct SND *p);
void func_001FB018(struct SND *p);
void func_0014F5A0(struct CM *p, float g);
void func_002581C0(struct CRef *, void *, void *);
void func_00129EC8(struct CRef *);
float func_0011CFE0(float g, float gMax);
float RadNormalize__Ff(float g);
qword func_0014FEC8(struct X *p);
qword func_0014FE58(struct X *p);
void func_0014E1A8(struct CM *p, qword a, qword b, float c, float d);

static __inline__ float GClamp(float g, float gMin, float gMax)
{
    if (g < gMin) return gMin;
    if (gMax < g) return gMax;
    return g;
}

static __inline__ float GClamp2(float g, float gMin, float gMax)
{
    if (g < gMin) g = gMin;
    else if (gMax < g) g = gMax;
    return g;
}

void func_00150160(struct X *p, void *pv, struct JOY *pjoy)
{
    float x, u, gPan, gTilt, dt;
    int f;
    struct CRef ref;
    func_0014FD40(p, pv, pjoy);
    x = p->g20;
    p->g24 = (D_002BD980.c0 + x * (D_002BD980.c1 + x * D_002BD980.c2)) * pjoy->g1074 * __builtin_fabsf(pjoy->g1074);
    x += p->g24 * D_002DDED0.dt;
    p->g20 = x = GClamp2(x, 0.0f, 1.0f);
    if (p->g24 != 0.0f && 0.0f < x && x < 1.0f) {
        struct SND *ps = p->p34;
        func_001FB2C0(ps, func_0011D948(D_002BBB68, __builtin_fabsf(p->g24)));
        ps = p->p34;
        func_001FB1F8(ps, func_0011D948(D_002BBB60, __builtin_fabsf(p->g24) * 0.5f + 0.5f));
        if (p->p34->c4E > 0)
            func_001FB060(p->p34);
    } else {
        if (p->p34->c4E <= 0)
            func_001FB018(p->p34);
    }
    u = 1.0f - p->g20;
    func_0014F5A0(&D_002DDF40, (1.0f - u) * p->g2C + u * p->g30);
    gPan = -__builtin_fabsf(pjoy->g103C) * pjoy->g103C * D_002BD990;
    gTilt = -__builtin_fabsf(pjoy->g1040) * pjoy->g1040 * D_002BD994;
    func_002581C0(&ref, D_003DD490.p3B4C, D_003D4A60);
    f = ref.m_n;
    func_00129EC8(&ref);
    if (f)
        gTilt = -gTilt;
    gPan = p->g18 + func_0011CFE0(gPan - p->g18, D_002BD998 * D_002DDED0.dt);
    gTilt = p->g1C + func_0011CFE0(gTilt - p->g1C, D_002BD99C * D_002DDED0.dt);
    dt = D_002DDED0.dt * 0.5f;
    {
        float gMin, gMax;
        GSmooth__FfffP3SMPPf(p->g10, p->g3C, dt, D_002BD9A0, &gMin);
        GSmooth__FfffP3SMPPf(p->g10, p->g40, dt, D_002BD9A0, &gMax);
        p->g18 = GClamp(gPan, gMin, gMax);
        GSmooth__FfffP3SMPPf(p->g14, p->g44, dt, D_002BD9A0, &gMin);
        GSmooth__FfffP3SMPPf(p->g14, p->g48, dt, D_002BD9A0, &gMax);
        p->g1C = GClamp(gTilt, gMin, gMax);
    }
    {
        float k = (1.0f - D_002DDF40.g20) * D_002BD9B0.gMin + D_002DDF40.g20 * D_002BD9B0.gMax;
        p->g10 += k * p->g18 * D_002DDED0.dt;
        p->g14 += k * p->g1C * D_002DDED0.dt;
    }
    if (6.2831855f <= p->g40 - p->g3C)
        p->g10 = RadNormalize__Ff(p->g10);
    func_0014E1A8(&D_002DDF40, func_0014FEC8(p), func_0014FE58(p), 0.0f, 0.0f);
    D_003DD490.g4750 = D_002DDED0.t;
}
