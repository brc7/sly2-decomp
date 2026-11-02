typedef int qword __attribute__((mode(TI)));
extern "C" {
void STACKSCOPE_ctor(void *p);
void STACKSCOPE_ctor_1AE4A0(void *p, int n);
void *PdteFindDetectorTarget(void *a, void *b);
void SetDetectorTargetState(void *a, void *b, int c);
qword func_001A62E8(void *p);
void IntersectSwBoundingBox(void *g, qword a, qword b, void *pfn, void *o, int *pi, int *pj);
int PsoHitTestLineObjects__FiP6VECTORT1iPP2SOP3LSG(int z, qword a, qword b, int i, int j, int k);
void func_00154568(void);
extern char D_003DD490[];
extern float D_002BF638;
}
class FRAME { public: FRAME() { STACKSCOPE_ctor(this); } ~FRAME() { STACKSCOPE_ctor_1AE4A0(this, 2); } };
struct E { int x0; int x4; };
struct F { char pad[0xF8]; int xf8; };
struct O { char pad[0xF64]; F *xf64; char pad2[0xFE0 - 0xF68]; O *xfe0; };
extern "C" O *D_003E1444;
struct P { void *x0; };
extern "C" void func_00192A28(P *p);
/** @sly2-annot addr=0x192A28
 * Purpose: For each detector-owning object in list D_003E1444 that currently tracks the player: if farther
 *   than D_002BF638 or line of sight blocked (IntersectSwBoundingBox + PsoHitTestLineObjects) reset its
 *   detector state (SetDetectorTargetState 0)
 * Info: proposed name ClearJtDetectionOutOfSight (T2); TU grab
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grab-0x191090-0x195950
 * @end-sly2-annot */
void func_00192A28(P *p)
{
    O *o;
    for (o = D_003E1444; o; o = o->xfe0) {
        E *e = (E *)PdteFindDetectorTarget(o->xf64, p->x0);
        if (!e || !e->x4 || (o->xf64->xf8 & 1)) continue;
        qword a = func_001A62E8(o);
        qword b = func_001A62E8(p->x0);
        qword d, t;
        float dist;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
        __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z\n\tvsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "+j"(d), "=&j"(t));
        __asm__("qmfc2.ni %0, %1" : "=r"(dist) : "j"(d));
        bool far = D_002BF638 < dist;
        if (far) {
            SetDetectorTargetState(o->xf64, e, 0);
            continue;
        }
        struct { char c; FRAME frame; int i, j; } x;

        IntersectSwBoundingBox(D_003DD490, a, b, (void *)func_00154568, o, &x.i, &x.j);
        if (!PsoHitTestLineObjects__FiP6VECTORT1iPP2SOP3LSG(0, a, b, x.i, x.j, 0)) continue;
        SetDetectorTargetState(o->xf64, e, 0);
    }
}
