struct M { int n0; int n4; int n8; };
struct T { char pad[0xF3C]; int nF3C; };
struct P { char pad[8]; int n8; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x2C]; struct P *p40; char pad3[0xC]; int n50; };
int func_00209518(void *p);
int BHVANIM_FHandleMessage(struct S *p, int msg, struct M *pm);
void func_0020C760(int n);
/** @sly2-annot addr=0x1752F0
 * Purpose: BHVSTRIKE message handler: ASEG events 0x2D/0x2E open/close the hit window (+0xF3C), 0x30 releases
 *   the leap; else BHVANIM
 * Info: proposed name BHVSTRIKE_FHandleMessage (T1); class BHVSTRIKE slot13; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVSTRIKE role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
int BHVSTRIKE_FHandleMessage(struct S *p, int msg, struct M *pm) {
    if (msg == 9 && p->p40 && func_00209518(p->p40) == pm->n0) {
        switch (pm->n8) {
        case 0x2D: p->pt->nF3C = 1; return 1;
        case 0x2E: p->pt->nF3C = 0; return 1;
        case 0x30: p->n50 = 0; func_0020C760(p->p40->n8); return 1;
        }
    }
    return BHVANIM_FHandleMessage(p, msg, pm);
}
