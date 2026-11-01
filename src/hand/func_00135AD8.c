struct VT { short d; short pad; void *pfn; };
struct O { char pad[0xC]; struct VT *pvt; char pad2[0x15C - 0x10]; int x15C; };
struct Q { int x0; void *p4; };
struct M { int x0; int x4; int x8; };
struct E { int x0; int x4; struct M *pm; };
struct P { char pad[0x1030]; int x1030; char pad1[0x16B4 - 0x1034]; struct Q *pq; char pad2[0x16CC - 0x16B8]; int x16CC;
    char pad3[0x1710 - 0x16D0]; char a1710[0x1824 - 0x1710]; struct E ae[6]; char pad4[0x186C - 0x186C]; struct O *po; };
void func_00140F90(struct P *p, int k, struct M *pm);
struct O *func_001CB250(void *pv, int n);
void func_001CB488(void *pv, struct O *po);
void func_001357C0(struct P *p, int n);
void func_00136ED0(void *pv, int k, struct M *pm);
void func_0018D698(struct P *p, struct E *pe, int n);
/** @sly2-annot addr=0x135AD8
 * Purpose: BENTLEY vt5: HandleStepplayerMessage then Bentley-specific msg 9 kinds (0x216 spawn object from
 *   template, ...)
 * Info: proposed name HandleBentleyMessage (T1); class BENTLEY slot5; TU bentley
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r2b_rest+verdicts_followups
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
void func_00135AD8(struct P *p, int k, struct M *pm)
{
    func_00140F90(p, k, pm);
    switch (k) {
    case 9:
        if (pm->x0 == p->x1030) {
            if (pm->x8 == 0x216) {
                struct O *po;
                p->po = func_001CB250(p->pq->p4, 0);
                func_001CB488(p->pq->p4, p->po);
                p->po->x15C = p->pq->x0;
                po = p->po;
                ((void (*)(void *, int))po->pvt[76].pfn)((char *)po + po->pvt[76].d, 0);
            }
        } else if (pm->x0 == p->x16CC) {
            if (pm->x8 == 0x243)
                func_001357C0(p, 0x80000);
        } else {
            func_00136ED0(p->a1710, 9, pm);
        }
        break;
    case 1: {
        struct E *pe = p->ae;
        int i;
        for (i = 0; i < 6; i++, pe++) {
            if (pe->pm == pm)
                func_0018D698(p, pe, 0);
        }
        break;
    }
    }
}
