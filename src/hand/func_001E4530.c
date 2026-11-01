struct G { int a; };
extern struct G D_003DD490;
extern struct G D_003DD040;
extern struct G D_002DE578;
extern float D_002BD850;
union U { int n; float g; };
struct P { int kind; int x4; int pad[2]; union U u10; union U u14; int x18; };
struct C { char pad[0x18]; int x18; };
struct O { char pad[0x74]; float g74; float g78; char pad2[0x80 - 0x7C]; float a[3]; int pad3; float gMin; float gMax; };
struct O *func_001E1DB8(struct C *pc, int n);
float func_00154C90(struct G *pg);
void *func_0015E938(struct G *pg, int k, int a, int b);
void *func_001F67E0(void *p, int n);
float func_001F11A8(void *p, int n);
float func_001FCFD0(struct G *pg, int n);
static __inline__ float GClamp(float g, float lo, float hi) { if (g < lo) return lo; if (g > hi) return hi; return g; }
/** @sly2-annot addr=0x1E4530
 * Purpose: Seconds a command takes: PlayAseg length/speed (difficulty-scaled), ApplySm ETA,
 *   Face/Rumble/AddObject/RemoveObject/HintBox duration arg, Line = dialogue length, Unk32 constant; 0 if
 *   fAsync
 * Info: proposed name GJsgCommandWait (T2); TU relay
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
float func_001E4530(struct C *pc, struct P *p)
{
    float g = 0.0f;
    if (p->x4) return g;
    switch (p->kind) {
    case 0xA: {
        struct O *po = func_001E1DB8(pc, p->u10.n);
        if (po) {
            float t = func_00154C90(&D_002DE578);
            float *pa = po->a; float s = pa[0] + t * (pa[1] + t * pa[2]); s = GClamp(s, po->gMin, po->gMax);
            g = po->g74 / (po->g78 * s);
        }
        break;
    }
    case 0xD:
        if (p->x18 != -1) {
            void *px = func_0015E938(&D_003DD490, 4, p->u10.n, pc->x18);
            if (px) {
                void *py = func_001F67E0(px, p->u14.n);
                if (py) g = func_001F11A8(py, p->x18);
            }
        }
        break;
    case 0x32:
        g = D_002BD850;
        break;
    case 0x23:
    case 0x33:
        g = p->u10.g;
        break;
    case 0x45:
        g = func_001FCFD0(&D_003DD040, p->u10.n);
        break;
    case 0x2C:
    case 0x2D:
    case 0x53:
        g = p->u14.g;
        break;
    }
    return g;
}
