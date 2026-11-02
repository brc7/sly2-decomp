typedef struct { float a, b; } F2;
struct G { char pad[0x24]; float x24; char pad2[0x60 - 0x28]; int x60; };
extern struct G D_002DDF40;
extern struct { float t; float dt; } D_002DDED0;
extern float D_003E1BE0;
extern F2 D_002BD950, D_002BD958, D_002BD960, D_002BD968, D_002BD970;
struct X {
    int x0; char pad[0xC]; float x10; float x14; int x18; int x1C; int x20; int x24; float x28; float x2C; float x30;
    int x34; int x38; F2 x3C; F2 x44; float x4C; float x50; int x54;
};
void ActivateCplcy(struct X *p);
void *PsndStartSound(int);
void SetSndSourceLo(void *, int);
void func_00258040(int *, void *);
void PauseSnd(int);
void func_0011BC88(void *, float *, float *);
int FIsBasicDerivedFrom__FP5BASIC3CID(int, int);
void *func_001A6318(int);
float func_0011D948(F2 *, float);
void SetCmNearClip__FP2CMf(struct G *, float);

/** @sly2-annot addr=0x14FF38
 * Purpose: Binocucom/look mode enter: start zoom sounds 0x26/0x25, pan/tilt from current camera (or focus),
 *   pan/tilt limits, uZoom=0, fov zoom range, near clip 49, cEnter++
 * Info: proposed name ActivateCplook (T1); TU cplcy
 * Notes: runtime3: CM+0x450 CPLOOK prio 7
 * Confidence: high [T1 0.90] - evidence: runtime (save-state RAM); semantic review; sources:
 *   r2_131F70+runtime
 * See: docs/engine/camera.md#5-key-functions, docs/engine/camera.md#open-questions,
 *   docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void ActivateCplook(struct X *p)
{
    struct G *pg;
    void *pv;
    float g;
    ActivateCplcy(p);
    pv = PsndStartSound(0x26);
    SetSndSourceLo(pv, 0);
    func_00258040(&p->x34, pv);
    {
        void *pv2 = PsndStartSound(0x25);
        SetSndSourceLo(pv2, 0);
        func_00258040(&p->x38, pv2);
    }
    PauseSnd(p->x34);
    pg = &D_002DDF40;
    func_0011BC88(&pg->x60, &p->x10, &p->x14);
    if (FIsBasicDerivedFrom__FP5BASIC3CID(p->x0, 0x34)) {
        p->x3C = D_002BD950;
        p->x44 = D_002BD958;
    } else {
        p->x3C = D_002BD968;
        p->x44 = D_002BD970;
        p->x4C = p->x10;
        p->x50 = p->x14;
        func_0011BC88(func_001A6318(p->x0), &p->x10, &p->x14);
        g = func_0011D948(&D_002BD960, p->x14);
        p->x14 = g;
        p->x3C.a += p->x10;
        p->x3C.b += p->x10;
        p->x44.a += g;
        p->x44.b += g;
        p->x44.a = func_0011D948(&D_002BD960, p->x14);
        p->x44.b = func_0011D948(&D_002BD960, p->x14);
    }
    p->x18 = 0;
    p->x1C = 0;
    p->x20 = 0;
    p->x24 = 0;
    p->x2C = D_002DDF40.x24 / p->x28;
    p->x30 = D_002DDF40.x24;
    SetCmNearClip__FP2CMf(&D_002DDF40, 49.0f);
    p->x54++;
    D_003E1BE0 = D_002DDED0.t;
}
