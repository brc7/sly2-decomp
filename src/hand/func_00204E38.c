typedef int qword __attribute__((mode(TI)));
typedef struct { short d; short pad; void *pfn; } VTE;
extern qword D_002A2DD0;
struct O { VTE *vt; };
struct X { int a; struct O *po; };
struct TABLE;
struct Q { char pad[0x14]; struct TABLE *table; char pad2[0x40 - 0x18]; float g40; };
int FAsegaRetracted(struct Q *q);
struct X *func_0012FF10(struct TABLE *table, void *owner, int kind, int check);
/** @sly2-annot addr=0x204E38
 * Purpose: If the ASEGA is live: find its translation channel for this object (PchnFindAseg kind 1, checked)
 *   and evaluate it (channel vt3) at t with the ASEGA effective speed (+0x40); else zero vector
 * Info: proposed name VecStepAsegaTranslation (T2); TU step
 * Notes: top50_round (0.50): Body. +0x40 is the effective svt (SetAsegaSpeed 0x131A00 stores svt*x48*x4C
 *   there). Callers use it for root motion (BHVANIM 0x15E events, UpdateMurray, PresetJtAccel, UpdateDancer)
 * Confidence: low (draft) [T2 0.50] - evidence: semantic review; sources: r7b_guard+top50_round
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
qword func_00204E38(void *p, struct Q *q, float g)
{
    qword r = D_002A2DD0;
    if (q && !FAsegaRetracted(q)) {
        struct X *x = func_0012FF10(q->table, p, 1, 1);
        if (x) {
            struct O *o = x->po;
            ((void (*)(void *, qword *, qword *, float, float))o->vt[3].pfn)((char *)o + o->vt[3].d, 0, &r, g, q->g40);
        }
    }
    return r;
}
