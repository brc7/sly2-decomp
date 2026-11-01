typedef int qword __attribute__((mode(TI)));
struct ALO;
struct CLOCK { float t; float dt; };
extern CLOCK D_002DDED0;
struct CM { char pad[0x1C]; float g1C; char pad2[0x90 - 0x20]; qword pos; };
extern CM D_002DDF40;
struct EMITO { void *palo; int f4; };
struct EMITB { int pad0; int f4; EMITO *pemito; };
struct RIPG { char pad[0x15C]; int dl; };
struct PRG { char pad[0x444]; int cpart; };
struct EMITTER {
    char pad0[0x10]; int x10; char pad1[0x38 - 0x14]; float g38;
    char pad2[0xE0 - 0x3C]; int fChanged; EMITB *pemitb; char padE8[8]; int k; int c;
    float gF8, gFC; char pad3[0x104 - 0x100]; float g104, g108, g10C, g110; int f114; int x118;
    int dl11C; char pad4[0x128 - 0x120]; EMITTER *pNext;
    char pad5[0x130 - 0x12C]; float g130, g134, g138, g13C; char pad6[0x144 - 0x140]; float g144;
    char pad7[0x14C - 0x148]; RIPG *pripg; PRG *pprg;
};
struct EMITX { PRG **ppprg; RIPG **ppripg; EMITTER *pemitter; };
extern "C" {
void func_001A5E28(EMITTER *p);
int FIsDlEmpty__FP2DL(int *pdl);
int func_001A61A0(EMITTER *p);
qword func_001A62E8(void *p);
float GRandInRange__Fff(float gMin, float gMax);
int func_0011A3C0(int *pdl);
void *func_002333B4(void *pv, int b, int cb);
void func_0015BD20(int c, EMITB *pemitb, EMITX *px);
}
void OnEmitterValuesChanged(EMITTER *p);
int FPausedEmitter(EMITTER *p);
void PauseEmitter(EMITTER *p, float dt);
void PauseEmitterIndefinite(EMITTER *p);
void ModifyEmitterParticles(EMITTER *p);

static inline float Dist2(qword a, qword b)
{
    qword d, t;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "+j"(d), "=&j"(t));
    float g;
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
}
static inline int FLess(float a, float b) { return a < b; }

void UpdateEmitter(EMITTER *p, float dt)
{
    if (p->fChanged)
        OnEmitterValuesChanged(p);
    func_001A5E28(p);
    if (p->x118 != -1)
        return;
    if (!FIsDlEmpty__FP2DL(&p->dl11C)) {
        EMITTER *pe;
        for (pe = (EMITTER *)p->dl11C; pe; pe = pe->pNext) {
            if (!func_001A61A0(pe))
                continue;
            if (!pe->x10)
                goto LActive;
            float r = pe->g38 * D_002DDF40.g1C;
            qword pos = func_001A62E8(pe);
            r = r * r;
            if (FLess(Dist2(pos, D_002DDF40.pos), r))
                goto LActive;
        }
        return;
    } else if (p->x10) {
        float r = p->g38 * D_002DDF40.g1C;
        qword pos = func_001A62E8(p);
        r = r * r;
        if (FLess(r, Dist2(pos, D_002DDF40.pos)))
            return;
    }
LActive:
    if (FPausedEmitter(p))
        return;

    int c = 0;
    switch (p->k) {
    case 3: {
        float g = p->g144 + p->g130 * p->g13C * D_002DDED0.dt;
        c = (int)g;
        p->g144 = g - (float)c;
        if (1.0f < (float)c) {
            int cLeft = p->c;
            if (cLeft >= 0) {
                if (cLeft <= c)
                    c = cLeft;
                p->c = cLeft - c;
            }
            if (p->f114) {
                PauseEmitterIndefinite(p);
                p->g130 = GRandInRange__Fff(p->gF8, p->gFC) * 60.0f;
            } else if (GRandInRange__Fff(0.0f, 1.0f) <= p->g104) {
                PauseEmitter(p, GRandInRange__Fff(p->g108, p->g10C));
                p->g130 = GRandInRange__Fff(p->gF8, p->gFC) * 60.0f;
            }
        }
        break;
    }
    case 2: {
        c = (int)p->g130;
        int cLeft = p->c;
        if (cLeft >= 0) {
            if (cLeft <= c)
                c = cLeft;
            p->c = cLeft - c;
        }
        if (p->f114)
            PauseEmitterIndefinite(p);
        else if (GRandInRange__Fff(0.0f, 1.0f) <= p->g104)
            PauseEmitter(p, GRandInRange__Fff(p->g108, p->g10C));
        p->g130 = GRandInRange__Fff(p->gF8, p->gFC);
        break;
    }
    case 0: {
        float g = p->g144 + p->g130 * p->g13C * D_002DDED0.dt;
        c = (int)g;
        p->g144 = g - (float)c;
        if (c != 0) {
            int cLeft = p->c;
            if (cLeft >= 0) {
                if (cLeft <= c)
                    c = cLeft;
                p->c = cLeft - c;
            }
        }
        break;
    }
    case 1: {
        int cWant = (int)(p->g110 * p->g13C);
        int cCur;
        if (!p->pemitb->f4)
            cCur = p->pprg ? p->pprg->cpart : 0;
        else
            cCur = p->pripg ? func_0011A3C0(&p->pripg->dl) : 0;
        int d = cWant - cCur;
        c = 0;
        int f = 0 < d;
        if (f)
            c = d;
        break;
    }
    }

    if (c) {
        int unused[4];
        EMITX x;
        func_002333B4(&x, 0, sizeof(x));
        x.pemitter = p;
        x.ppprg = &p->pprg;
        x.ppripg = &p->pripg;
        EMITO *pemito = p->pemitb->pemito;
        void *palo = pemito->palo;
        if (!palo && !pemito->f4)
            pemito->palo = p;
        func_0015BD20(c, p->pemitb, &x);
        p->pemitb->pemito->palo = palo;
    }

    if (D_002DDED0.t >= p->g138) {
        p->g130 = GRandInRange__Fff(p->gF8, p->gFC);
        p->g138 = D_002DDED0.t + p->g134;
    }
    ModifyEmitterParticles(p);
}
