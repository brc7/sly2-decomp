struct T { char pad[0x30]; int n30; };
struct S { char pad[0x10]; void *p10; int pad2; struct T *p18; char pad3[0x14]; int n30; char pad4[0xC]; int n40; };
void func_001EEF98(struct S *p);
int func_00209098(void *p);
void func_0017B9C0(struct S *p);
int func_001EF0A8(struct S *p);
void func_0017B8B0(struct S *p, int n);
int func_00175810(struct T *p);
/** @sly2-annot addr=0x17BD78
 * Purpose: BHVTARGETS Update: re-choose and (re)engage designated targets; hands-off pause
 * Info: proposed name UpdateBhv178 (T2); class BHV178 slot10; TU guardbhv
 * Notes: BHV class word normalised: UpdateBhvtargets -> UpdateBhv178
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017BD78(struct S *p) {
    int f, s;
    func_001EEF98(p);
    f = func_00209098(p->p10);
    func_0017B9C0(p);
    while (!func_001EF0A8(p)) {
        s = p->n30;
        switch (s) {
        case 0:
            if (f) func_0017B8B0(p, 2);
            else if (p->n40) func_0017B8B0(p, 1);
            break;
        case 1:
            if (f) func_0017B8B0(p, 2);
            else if (!p->n40) func_0017B8B0(p, 0);
            else if (p->n40 != p->p18->n30) { func_0017B8B0(p, -1); func_0017B8B0(p, 1); }
            else if (func_00175810(p->p18)) func_0017B8B0(p, 0);
            break;
        case 2:
            if (!f) func_0017B8B0(p, 0);
            break;
        }
        if (s != p->n30) continue;
        break;
    }
}
