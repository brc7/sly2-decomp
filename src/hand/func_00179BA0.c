struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct S { char pad[0x10]; void *p10; int pad2; void *p18; char pad3[0x14]; float f30; float f34; int n38; char pad4[4]; int n40; };
void func_001EEF98(struct S *p);
int func_00209098(void *p);
int func_001EF0A8(struct S *p);
void func_00179D30(struct S *p, int n);
int func_00171458(void *p);
/** @sly2-annot addr=0x179BA0
 * Purpose: BHVPATROL Update: alternate walking the script for a random time and standing idle for a random
 *   time; stand forever when the script ends; hands-off pause
 * Info: proposed name UpdateBhvpatrol (T1); class BHVPATROL slot10; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVPATROL role on live guards (S02-S41)
 * Confidence: high [T1 0.80] - evidence: runtime (save-state RAM); sources: r9_bhv_screen+runtime(class role)
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_00179BA0(struct S *p) {
    int f, s;
    func_001EEF98(p);
    f = func_00209098(p->p10);
    while (!func_001EF0A8(p)) {
        s = p->n38;
        switch (s) {
        case 1:
            if (f) func_00179D30(p, 3);
            else if (D_002DDED0.t > p->f34) func_00179D30(p, 0);
            break;
        case 2:
            if (f) func_00179D30(p, 3);
            break;
        case 0:
            if (f) func_00179D30(p, 3);
            else if (D_002DDED0.t > p->f30) func_00179D30(p, 1);
            else if (func_00171458(p->p18)) { p->n40 = 0; func_00179D30(p, 2); }
            break;
        case 3:
            if (!f) {
                if (p->n40) func_00179D30(p, 1);
                else func_00179D30(p, 2);
            }
            break;
        }
        if (p->n38 != s) continue;
        break;
    }
}
