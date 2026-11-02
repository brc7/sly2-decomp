typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern float D_002BE6C0;
struct G { char pad[0x4020]; void *p4020; };
extern struct G D_003DD490;
struct R { char pad[0x54]; int n54; };
struct T { char pad[0x208]; int n208; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x1C]; int n30; int pad3; float g38; };
void UpdateBhv(struct S *p);
void func_0017A8C8(struct S *p, int n);
qword func_001A62E8(void *p);
struct R *func_00156698(void *p, qword q, struct T *t);
/** @sly2-annot addr=0x17A7F8
 * Purpose: BHVTHROWN Update: end when landed (+0x208) in states 2/3; emit yelling noise periodically while
 *   airborne
 * Info: proposed name UpdateBhv176 (T2); class BHV176 slot10; TU guardbhv
 * Notes: BHV class word normalised: UpdateBhvthrown -> UpdateBhv176
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017A7F8(struct S *p) {
    int s;
    UpdateBhv(p);
    do {
        s = p->n30;
        if (s < 4) { if (s >= 2 && p->pt->n208) func_0017A8C8(p, -1); }
    } while (s != p->n30);
    if (s == 1 || s == 4) {
        if (D_002DDED0.t - p->g38 > D_002BE6C0) {
            struct G *g = &D_003DD490;
            p->g38 = D_002DDED0.t;
            func_00156698(g->p4020, func_001A62E8(p->pt), p->pt)->n54 = 3;
        }
    }
}
