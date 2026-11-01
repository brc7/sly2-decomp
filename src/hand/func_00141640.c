/* VU0 length via asm; qmfc2 into an int then explicit mtc1 (gives the original GPR/FPR allocation). */
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } VU;
struct VTE { short d; short pad; void *pfn; };
struct XP { char pad[0x10]; VU pos; char pad2[0x34 - 0x20]; float g34; };
struct JT {
    char pad[0xC]; struct VTE *vt; char pad1[0x2C0 - 0x10]; char ax2C0[0x2DC - 0x2C0]; int x2DC;
    char pad2[0x434 - 0x2E0]; int x434; char pad3[0x4C0 - 0x438]; char ax4C0[0xC]; int x4CC;
    char pad4[0xDF0 - 0x4D0]; char axDF0[4]; int xDF4; char pad5[0x1044 - 0xDF8]; int x1044; int pad6;
    float x104C; float x1050; char pad7[0x1060 - 0x1054]; VU x1060; qword x1070;
};
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct GS { char pad[0x260]; struct JT *p260; char pad2[0x300 - 0x264]; int x300; };
extern struct GS D_002DDF40;
extern float D_002BCC98, D_002BCC9C, D_002BCCA0, D_002BCCA4, D_002BCCA8, D_002BCCAC, D_002BCCB0, D_002BCCB4;
extern char D_002BCCB8[], D_002BCCC8[], D_002BCCD8[];
extern struct { float a, b, c; } D_002BCCF0;
extern struct { float lo, hi; } D_002BBB60;
extern qword D_002A2DD0;
void func_001F58F0(struct JT *, struct XP *);
qword func_001A62E8(void *);
float GSmooth__FfffP3SMPPf(float g, float gTarget, float dt, void *pv, int n);
qword func_0011F088(qword a, qword b, float dt, void *pv, int n);
void func_0019C8B0(void *, struct XP *);
void func_0018A4E8(void *, struct XP *);
void func_001A2160(void *, struct XP *);

static __inline__ float GPoly(float x, float a, float b, float c) { return a + x * (b + x * c); }
static __inline__ float GClamp(float g, float lo, float hi)
{
    float r;
    if (g < lo)
        r = lo;
    else if (hi < g)
        r = hi;
    else
        r = g;
    return r;
}

void func_00141640(struct JT *pjt, struct XP *pxp)
{
    func_001F58F0(pjt, pxp);
    if (pjt->x1044) {
        pxp->pos.q = pjt->x1060.q;
        pjt->x104C = pjt->x1060.f[2];
    } else {
        VU v;
        float z, lo, hi, g;
        struct VTE *e;
        { qword q = func_001A62E8(pjt); __asm__("pextuw %0, $0, %1" : "=r"(z) : "r"(q)); }
        e = &pjt->vt[97];
        ((void (*)(void *, VU *))e->pfn)((char *)pjt + e->d, &v);
        if (pjt->x434) {
            lo = D_002BCCA0;
            hi = D_002BCCA4;
        } else if (pjt->x2DC == 0x16) {
            lo = D_002BCCB0;
            hi = D_002BCCB4;
        } else {
            lo = D_002BCCA8;
            hi = D_002BCCAC;
        }
        g = GClamp(v.f[2], z + lo, z + hi);
        pjt->x104C = g;
        if (D_002DDF40.x300 != 0 || D_002DDF40.p260 != pjt) {
            pxp->pos.f[2] = g;
        } else {
            void *pv = pjt->x434 ? D_002BCCC8 : D_002BCCB8;
            pxp->pos.f[2] = GSmooth__FfffP3SMPPf(pjt->x1050, pjt->x104C, D_002DDED0.dt, pv, 0);
            pxp->pos.f[2] = GClamp(pxp->pos.f[2], z + D_002BCC98, z + D_002BCC9C);
        }
        pxp->pos.f[0] = v.f[0];
        pxp->pos.f[1] = v.f[1];
    }
    {
        qword vel = func_0011F088(pjt->x1070, D_002A2DD0, D_002DDED0.dt, D_002BCCD8, 0);
        qword r;
        pjt->x1070 = vel;
        __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(pxp->pos.q), "j"(vel));
        pxp->pos.q = r;
        pjt->x1060.q = r;
    }
    pjt->x1050 = pxp->pos.f[2];
    if (pjt->x2DC != -1)
        func_0019C8B0(pjt->ax2C0, pxp);
    if (pjt->x4CC != -1)
        func_0018A4E8(pjt->ax4C0, pxp);
    if (pjt->xDF4 != -1)
        func_001A2160(pjt->axDF0, pxp);
    {
        struct VTE *e = &pjt->vt[34];
        register qword w __asm__("vf3") = ((qword (*)(void *))e->pfn)((char *)pjt + e->d);
        register qword l1 __asm__("vf1");
        register qword r2 __asm__("vf2");
        float s, k;
        __asm__("vmul.xyz %1, %2, %2\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(r2), "=&j"(l1) : "j"(w));
        __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(l1) : "j"(l1));
        { int t; __asm__("qmfc2.ni %0, %1" : "=r"(t) : "j"(l1)); __asm__("mtc1 %1, %0" : "=f"(s) : "r"(t)); }
        k = GPoly(s, D_002BCCF0.a, D_002BCCF0.b, D_002BCCF0.c);
        pxp->g34 *= GClamp(k, D_002BBB60.lo, D_002BBB60.hi);
    }
}
