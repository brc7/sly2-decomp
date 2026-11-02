struct VTE { short d; short pad; void (*pfn)(void *, ...); };
struct O { int pad[3]; struct VTE *vt; };
struct E { int prio; struct O *po; struct O *poTarget; int b; };
struct CM {
    char pad[0x20]; float g20; float g24; float g28; float g2C;
    char pad1[0x260 - 0x30]; struct O *po260; char pad2[0x270 - 0x264]; int x270[36];
    int x300; int pad3[2]; int c; struct E ae[8];
    char pad4[0x620 - 0x390]; float g620; float g624;
};
struct G { char pad[0x3E38]; int x3E38; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern float D_002DDEE0;
extern int D_002DFBC0;
extern char D_002DFC00[];
extern char D_002E0D00[];
extern struct G D_003DD490;
extern char D_002BD810[];
void ResetCmAdjust(struct CM *p, void *pv);
void AdjustCmCpdefi(struct CM *p, struct O *po, void *pv);
void SetCmFov__FP2CMf(struct CM *p, float g);
float GSmoothA__FffffP4SMPAPf(float g, float h, float i, float dt, float *pa, float *pb);
void RecalcCmFrustrum__FP2CM(struct CM *p);
float GSmooth__FfffP3SMPPf(float g, float gTarget, float dt, void *pv, float *pr);
/** @sly2-annot addr=0x14D478
 * Purpose: Per-frame camera update: for the top policy of acpr[] (+0x310), fetch focus CPDEFI via focus->vt50
 *   GetCpdefi into cm+0x270, ResetCmAdjust/AdjustCmCpdefi, then policy vt6 Update(pcpdefi, joy); then fov
 *   smoothing (SetCmFov if nCut else GSmoothA+RecalcCmFrustrum); clears nCut; smooths +0x620
 * Info: proposed name UpdateCmLast (T1); TU cm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void UpdateCmLast__FP2CM(struct CM *p)
{
    struct O *po;
    struct O *poPrev;
    struct O *poLast;
    if (p->c > 0) {
    poPrev = 0;
    poLast = 0;
    while ((po = p->ae[0].po) != poPrev) {
        struct O *poT = p->ae[0].poTarget;
        void *pv;
        if (poT) {
            pv = p->x270;
            if (poT != poLast) {
                struct VTE *e = &poT->vt[50];
                e->pfn((char *)poT + e->d, pv);
                if (poPrev || poT != p->po260)
                    ResetCmAdjust(p, pv);
                AdjustCmCpdefi(p, poT, pv);
                p->po260 = poT;
                poLast = poT;
            }
        } else {
            p->po260 = 0;
            pv = 0;
            poLast = 0;
        }
        {
            struct VTE *e = &po->vt[6];
            e->pfn((char *)po + e->d, pv, (D_002DFBC0 & 1) ? D_002DFC00 : D_002E0D00);
        }
        poPrev = po;
        if (p->c <= 0)
            break;
    }
    }
    if (p->x300) {
        SetCmFov__FP2CMf(p, p->g24);
    } else if (p->g24 != p->g20) {
        p->g20 = GSmoothA__FffffP4SMPAPf(p->g20, p->g28, p->g24, D_002DDED0.dt, &p->g2C, &p->g28);
        RecalcCmFrustrum__FP2CM(p);
    }
    p->x300 = 0;
    if (D_003DD490.x3E38)
        p->g620 = GSmooth__FfffP3SMPPf(p->g620, p->g624, D_002DDEE0, D_002BD810, 0);
}
