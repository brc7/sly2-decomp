struct M { int n0; int n4; int n8; };
struct T { char pad[0x5C]; int n5C; char pad2[0x1A4 - 0x60]; int n1A4; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x24]; int n38; char pad3[4]; void *p40; };
int func_00209518(void *p);
int func_00174BD0(struct S *p, int msg, struct M *pm);
void func_002303A0(struct T *t, int a, int b);
/** @sly2-annot addr=0x1750E0
 * Purpose: BHVIDLE message handler: ASEG events 0x29/0x2A toggle ground flag and XF role, else BHVANIM
 *   handler
 * Info: proposed name BHVIDLE_FHandleMessage (T1); class BHVIDLE slot13; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVIDLE role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
int func_001750E0(struct S *p, int msg, struct M *pm) {
    if (msg == 9 && p->p40 && func_00209518(p->p40) == pm->n0) {
        switch (pm->n8) {
        case 0x29: p->pt->n1A4 = 1; func_002303A0(p->pt, p->pt->n5C, 2); return 1;
        case 0x2A: p->pt->n1A4 = 0; if (!p->n38) func_002303A0(p->pt, p->pt->n5C, 1); return 1;
        }
    }
    return func_00174BD0(p, msg, pm);
}
