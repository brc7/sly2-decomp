typedef int qword __attribute__((mode(TI)));
struct T { char pad[0xF8]; int xF8; };
struct SO;
struct AL { int cb; int cbAlign; AL(int c, int a) { cb = c; cbAlign = a; } };
extern "C" {
void func_001AE478(void *p);
void func_001AE4A0(void *p, int n);
qword func_00153E60(T *p);
void *func_001AE318(AL *pal);
qword func_001A6F00(SO *pso, int z, qword v);
void func_0020FAA0(void *g, qword a, qword b, void *pfn, void *o, int *pi, int *pj);
int func_0014CC58(int z, qword a, qword b, int i, int j, int k);
void func_00154568(void);
void func_00154528(void);
extern char D_003DD490[];
extern qword D_002BDBF0[];
int func_001545F0(T *p, SO *pso);
}
class FRAME { public: FRAME() { func_001AE478(this); } ~FRAME() { func_001AE4A0(this, 2); } };


/** @sly2-annot addr=0x1545F0
 * Purpose: Tests whether the line from the detector eye to the target is blocked by objects
 *   (PsoHitTestLineObjects with the filter callbacks)
 * Info: proposed name FDetectorLineBlocked (T2); TU detector
 * Notes: names_v2 C proposed FCheckStepguardEnemyHidden__FP9STEPGUARD (0.438)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r3_1539A8
 * See: docs/reference/functions/guards-ai.md#detector-0x153d58-0x154a60
 * @end-sly2-annot */
int func_001545F0(T *p, SO *pso)
{
    FRAME frame;
    int i;
    qword *ppos = D_002BDBF0;
    qword q = func_00153E60(p);
    AL al(0x10, 0x10);
    qword *apos = (qword *)func_001AE318(&al);
    int ii, jj;
    qword vmax = q;
    qword vmin = q;
    for (i = 0; i < 1; i++) {
        qword t = func_001A6F00(pso, 0, ppos[i]);
        apos[i] = t;
        __asm__("vmini.xyz %0, %0, %1" : "+j"(vmin) : "j"(t));
        __asm__("vmax.xyz %0, %0, %1" : "+j"(vmax) : "j"(t));
    }
    void *pfn = (void *)func_00154568;
    if (p->xF8 & 4) pfn = (void *)func_00154528;
    func_0020FAA0(D_003DD490, vmin, vmax, pfn, p, &ii, &jj);
    bool f;
    for (i = 0; (f = i < 1); i++)
        if (!func_0014CC58(0, q, apos[i], ii, jj, 0)) break;
    return !f;
}
