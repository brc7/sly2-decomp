struct CRef { int m_tagk; void *pv; };
struct N;
struct SP { struct N *p; };
struct N { int cref; int x4; struct CRef ref; struct SP sp; };
struct P { void *p0; };
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
extern char D_003DD490[];
extern char D_004520A8[];
extern int D_0027A1D8[];
extern int D_0027A10C[];
int func_00127DB8(void *p, int k, int n);
struct CRef *func_00200DA0(struct CRef *ret, int x, int *pn, int y);
struct SP *func_00132ED8(struct SP *ret, struct P *p, int k, struct CRef *pref);
void ScheduleSwSpliceCallback(void *pg, struct P *p, struct N *pn, float t);
void EvalSpliceEventCallback(struct P *p, struct N *pn, void *p0, int z);
void func_00256D40(struct SP *psp, int n);
void func_00129EC8(struct CRef *pref);
void func_002550F8(void *pv);
void func_0011CD98(void *pool, void *pv);
/** @sly2-annot addr=0x1331F8
 * Purpose: BASIC vt3: if the frame binds symidEvent, build args from s_aotypEvtParm[s_aiEvtParm[ev]] and
 *   either schedule (fPost) or evaluate now
 * Info: proposed name HandleBasicSpliceEvent (T1); class SW slot3; TU basic
 * Notes: superseded: HandleLoSpliceEvent (arbitration) | arbitration knock-on rename: CONFIRMED static:
 *   slot-3 pfn in 50+ vtables incl. SW (not an LO), i.e. owner class BASIC. Body = Sly1 HandleLoSpliceEvent
 *   (frame binding lookup). Needed so the
 * Confidence: mid (draft) [T1 0.75] - evidence: Sly 1 / Lilly reference; arbitration; sources:
 *   r2b_rest+arbitration
 * See: docs/engine/object-model.md#2-the-lo-virtual-interface, docs/engine/object-model.md#9-key-functions,
 *   docs/engine/splice-scripting.md#5-objects-and-scripts
 * @end-sly2-annot */
void HandleBasicSpliceEvent(struct P *p, int k, int f, int x, int y, int z)
{
    if (p->p0 && func_00127DB8(p->p0, k, 1)) {
        struct CRef ref;
        struct SP sp;
        func_00200DA0(&ref, x, &D_0027A10C[D_0027A1D8[k]], y);
        func_00132ED8(&sp, p, k, &ref);
        if (f)
            ScheduleSwSpliceCallback(D_003DD490, p, sp.p, D_002DDED0.t);
        else
            EvalSpliceEventCallback(p, sp.p, p->p0, z);
        {
            struct N *pt = sp.p;
            if (pt && --pt->cref == 0) {
                func_00256D40(&pt->sp, 2);
                func_00129EC8(&pt->ref);
                func_002550F8(pt);
                func_0011CD98(D_004520A8, pt);
            }
        }
        func_00129EC8(&ref);
    }
}
