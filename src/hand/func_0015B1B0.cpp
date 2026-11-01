typedef int qword __attribute__((mode(TI)));
struct MATRIX3 { float mat[3][4]; } __attribute__((aligned(16)));
struct EMITTER;
struct SHD { void *p0; int x4; int x8; int pad; int x10; };
struct EMITTER {
    char pad0[8];
    SHD *pshd;
    char pad1[0xB0 - 0xC];
    qword qB0;
    char pad2[0xC0 - 0xC0];
    int xC0;
    int xC4;
    char pad3[0xD8 - 0xC8];
    float gD8, gDC;
    char pad4[4];
    float gE4, gE8;
};
class PART {
public:
    char pad0[0x14];
    float x14, x18;
    int x1C;
    char pad20[4];
    float x24, x28;
    char pad2C[4];
    qword x30;
    MATRIX3 mat40;
    char pad70[0x20];
    qword x90;
    char abA0[0xEC - 0xA0];
    virtual void Set(qword pos, float g, int n);
    virtual void Init(EMITTER *pemit);
};
class SO {
public:
    char pad[0xC];
    virtual void V1();
    virtual void V2();
    virtual void V3();
    virtual void V4();
    virtual void V5();
    virtual void V6();
    virtual void V7();
    virtual void V8();
    virtual void V9();
};
struct EMITX { int x0; SO **ppso; void *p8; };
extern "C" {
extern char D_003DD490[];
SO *func_001CDCD0(void *p, int n);
void func_001CDE70(SO *pso, EMITTER *pemit);
void SubscribeLoObject(SO *pso, void *p);
PART *func_001CE888(void *p, int n, SO *pso);
float GRandInRange__Fff(float a, float b);
void func_0015BF40(void *p, EMITTER *pemit);
MATRIX3 *func_001A6318(void *p);
int FIsDlEmpty__FP2DL(void *p);
}
extern "C" void func_0015B1B0(EMITTER *pemit, EMITX *px, int c, qword *apos, qword *avel, float *at0, float *at1)
{
    SO *pso = 0;
    int i;
    PART *ppart;
    if (px && px->ppso)
        pso = *px->ppso;
    if (!pso) {
        pso = func_001CDCD0(D_003DD490, pemit->xC4);
        if (!pso)
            return;
        if (px && px->ppso)
            *px->ppso = pso;
        func_001CDE70(pso, pemit);
        if (px && px->p8)
            SubscribeLoObject(pso, px->p8);
    }
    if (!pso)
        return;
    for (i = 0; i < c && (ppart = func_001CE888(D_003DD490, pemit->xC0, pso)) != 0; i++) {
        ppart->Set(apos[i], GRandInRange__Fff(pemit->gD8, pemit->gDC), pemit->pshd->x10);
        if (pemit->gE4 != 0.0f || pemit->gE8 != 0.0f) {
            ppart->x24 = pemit->gE4;
            ppart->x28 = pemit->gE8;
        }
        ppart->x90 = avel[i];
        ppart->x14 = at0[i];
        ppart->x18 = at1[i] - at0[i];
        ppart->x30 = pemit->qB0;
        func_0015BF40(ppart->abA0, pemit);
        ppart->x1C = pemit->pshd->x8;
        if (pemit->pshd->p0)
            ppart->mat40 = *func_001A6318(pemit->pshd->p0);
        ppart->Init(pemit);
    }
    if (FIsDlEmpty__FP2DL((char *)pso + 0x15C))
        pso->V9();
}
