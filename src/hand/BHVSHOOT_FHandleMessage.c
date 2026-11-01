struct M { int n0; int n4; int n8; };
struct U { char pad[0x40]; void *p40; };
struct T { char pad[0xF60]; int nF60; };
struct S { char pad[0x10]; struct T *pt; int pad2; struct U *p18; char pad3[0x14]; int n30; };
int func_00209518(void *p);
int BHV_FHandleMessage(struct S *p, int msg, struct M *pm);
/** @sly2-annot addr=0x17B690
 * Purpose: BHVSHOOT message handler: ASEG events 0x31/0x32 open/close the firing window (+0xF60)
 * Info: proposed name BHVSHOOT_FHandleMessage (T1); class BHVSHOOT slot13; TU guardbhv
 * Notes: BHV class word normalised: BHVSHOOT_FHandleMessage -> BHV173_FHandleMessage | v3 BHV class word
 *   (runtime6 P2: BHV194 = follow, BHV168 = walk-to-goal child): BHV173_FHandleMessage ->
 *   BHVSHOOT_FHandleMessage | runtime7a: BHVSHOOT role confirmed
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)+runtime
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
int BHVSHOOT_FHandleMessage(struct S *p, int msg, struct M *pm) {
    if (msg == 9 && !p->n30 && p->p18->p40 && func_00209518(p->p18->p40) == pm->n0) {
        switch (pm->n8) {
        case 0x31: p->pt->nF60 = 1; return 1;
        case 0x32: p->pt->nF60 = 0; return 1;
        }
    }
    return BHV_FHandleMessage(p, msg, pm);
}
