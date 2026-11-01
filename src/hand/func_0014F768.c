typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
typedef struct { qword r[3]; } M3;
typedef struct { qword r[4]; } M4;
struct CM { char pad[0x60]; M3 mat; qword pos; char pad2[0x100 - 0xA0]; char m100[0x520]; float x620; };
void func_00256D18(qword *p, qword v);
void func_0014D8F8(M4 *pm, void *pv, int n);
static __inline__ void LoadM4(M4 *pm, M3 *pmat)
{
    pm->r[0] = pmat->r[0];
    pm->r[1] = pmat->r[1];
    pm->r[2] = pmat->r[2];
    ((int *)&pm->r[1])[3] = 0;
    ((int *)&pm->r[2])[3] = 0;
    ((int *)&pm->r[0])[3] = 0;
    ((float *)&pm->r[3])[3] = 1.0f;
}
/** @sly2-annot addr=0x14F768
 * Purpose: Builds a view-proj from CM::mat with pos offset along the right axis by +/- rUnk620 (k=1/2) via
 *   CombineEyeLookAtProj into pm (proj = pv or matProj)
 * Info: proposed name BuildCmEyeViewProj (T2); TU cm
 * Confidence: low (draft) [T2 0.40] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/rendering.md#8-blips-hud-screen-effects,
 *   docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void func_0014F768(struct CM *p, int k, void *pv, int n, M4 *pm)
{
    U r, g;
    float f = 0.0f;
    switch (k) {
    case 1: f = p->x620; break;
    case 2: f = -p->x620; break;
    }
    __asm__("qmtc2.ni %1, %0" : "=j"(g.q) : "r"(f));
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r.q) : "j"(p->pos), "j"(p->mat.r[1]), "j"(g.q));
    func_00256D18(&pm->r[3], r.q);
    LoadM4(pm, &p->mat);
    func_0014D8F8(pm, pv ? pv : p->m100, n);
}
