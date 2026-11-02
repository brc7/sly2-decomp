// InitCm: near/far and LM init via inline helpers (parameter pseudos give the original FP register choice).
typedef int qword __attribute__((mode(TI)));
struct RGBA { unsigned char r, g, b, a; RGBA(unsigned int n) { *(unsigned int *)this = n; } };
struct V3 { float x, y, z; };
struct LM { float g0; int n4; float g8; float gC; };
struct CM {
    char pad[0x10]; int x10; int x14; float g18; float g1C; float g20; float g24; int pad28;
    float g2C; int x30; float g34; float g38; float g3C; int pad40[3]; float g4C; float g50; int pad54[3];
    float mat[16]; char padA0[0x240 - 0xA0]; V3 v240; RGBA rgba;
    char pad250[0x30C - 0x250]; int c30C; char pad310[0x3A0 - 0x310]; int x3A0[4]; int x3B0[40]; int x450[22];
    int x4A8[6]; int x4C0[16]; int x500[8]; int x520[52]; int x5F0[10]; float g618; int x61C;
};
extern "C" {
extern float D_002BD81C, D_002BD820, D_002BD824;
extern qword D_002BD830;
extern qword D_002BD840;
void *func_002333B4(void *pv, int b, int cb);
void RecalcCmFrustrum__FP2CM(CM *p);
void func_0014E1A8(CM *p, qword pos, qword tgt, float da, float db);
void CutCm__FP2CM(CM *p);
void InitCmpart1(void *p);
void func_002219C8(void *p);
void InitCplcy(void *p);
void SetCmPolicy(CM *p, int prio, void *po, int a, int b);
}
static inline void SetNF(CM *p, float n, float f) { p->g4C = n; p->g50 = f; }
static inline void InitLm(LM *plm, float g0, float g8, float gC) { plm->g0 = g0; plm->gC = gC; plm->g8 = g8; plm->n4 = 0; }
/** @sly2-annot addr=0x14D6D8
 * Purpose: Camera init: rgba fog 0x80303030, aspect 1.2987, near/far 100/100000, identity mat,
 *   RecalcCmFrustrum, SetCmLookAt, CutCm, Init on each embedded policy, push default policy (+0x3A0) with
 *   SetCmPolicy(prio 0), tJolt=-10
 * Info: proposed name InitCm (T1); TU cm
 * Notes: names_v2 v1 proposed AdjustCmCpdefi__FP2CMP2SOfP6CPDEFI (0.638)
 * Confidence: high [T1 0.90] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
extern "C" void InitCm__FP2CM(CM *p)
{
    static RGBA s_rgba(0x80303030);
    p->v240.x = D_002BD81C;
    p->v240.y = D_002BD820;
    p->v240.z = D_002BD824;
    p->g3C = 1.29870152f;
    SetNF(p, 100.0f, 100000.0f);
    p->g1C = 1.0f;
    p->g18 = 1.0f;
    p->g24 = 1.0f;
    p->g20 = 1.0f;
    p->rgba = s_rgba;
    InitLm((LM *)&p->g2C, 0.5f, 0.1f, 10.0f);
    float *pm = p->mat;
    func_002333B4(pm, 0, 0x40);
    pm[15] = 1.0f;
    pm[10] = 1.0f;
    pm[5] = 1.0f;
    pm[0] = 1.0f;
    RecalcCmFrustrum__FP2CM(p);
    func_0014E1A8(p, D_002BD830, D_002BD840, 0.0f, 0.0f);
    CutCm__FP2CM(p);
    p->c30C = 0;
    InitCmpart1(p->x3A0);
    func_002219C8(p->x3B0);
    InitCplcy(p->x450);
    InitCplcy(p->x4A8);
    InitCplcy(p->x4C0);
    InitCplcy(p->x500);
    InitCplcy(p->x520);
    InitCplcy(p->x5F0);
    p->x10 = 0;
    p->x14 = 0xFF;
    SetCmPolicy(p, 0, p->x3A0, 0, 0);
    p->x61C = -1;
    p->g618 = -10.0f;
}
