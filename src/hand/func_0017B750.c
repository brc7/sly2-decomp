struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x28]; int n38; int n3C; };
struct T { char pad[0xF60]; int nF60; };
struct S { char pad[0x10]; struct T *pt; int pad2; char a18[0x18]; int n30; float g34; int n38; };
void func_001EF0D8(void *p);
struct O *func_00205438(void *p, int n);
/** @sly2-annot addr=0x17B750
 * Purpose: BHVSHOOT state: 0 = push BHVANIM flag 0x11 aimed at target and set +0xF60
 * Info: proposed name SetBhvshootState (T1); TU guardbhv
 * Notes: BHV class word normalised: SetBhvshootState -> SetBhv173State | v3 BHV class word (runtime6 P2:
 *   BHV194 = follow, BHV168 = walk-to-goal child): SetBhv173State -> SetBhvshootState | runtime7a: BHVSHOOT
 *   state field +0x30 observed (S67)
 * Confidence: mid (draft) [T1 0.60] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)+runtime
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017B750(struct S *p, int n) {
    if (n == p->n30) return;
    func_001EF0D8(p->a18);
    if (!p->n30) p->pt->nF60 = 0;
    p->n30 = n;
    p->g34 = D_002DDED0.t;
    if (!n) {
        struct O *o = func_00205438(p->pt, 0x9D);
        struct VTE *e;
        o->n3C = 0x11;
        o->n38 = p->n38;
        e = &o->vt[8];
        e->pfn((char *)o + e->d);
        p->pt->nF60 = 1;
    }
}
