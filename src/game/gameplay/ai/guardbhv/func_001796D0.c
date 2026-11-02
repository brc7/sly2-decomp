struct E { int n; int a, b, c; };
struct T { char pad[0xA70]; struct E ae[1]; };
struct T2 { char pad[0xF64]; void *pF64; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x2C]; struct E *p40; char pad3[0xC]; int n50; char pad4[8]; void *p5C; };
extern char D_002DEB30[];
int func_001F78A0(void *p);
void StartBhvhit(struct S *p);
void BroadcastGameEvent(void *g, int n, void *p);
void SetDetectorTargetStateById(void *p, int a, int b);
/** @sly2-annot addr=0x1796D0
 * Purpose: BHVHURT Start: reaction entry from table +0xA70 by zap kind, BHVHIT Start, broadcast game event
 *   0x15, mark detector target
 * Info: proposed name StartBhv161 (T2); class BHV161 slot8; TU guardbhv
 * Notes: BHV class word normalised: StartBhvhurt -> StartBhv161
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_001796D0(struct S *p) {
    p->p40 = &p->pt->ae[func_001F78A0(p->p5C)];
    if (p->p40->n == -1) p->p40 = 0;
    StartBhvhit(p);
    BroadcastGameEvent(D_002DEB30, 0x15, p->pt);
    SetDetectorTargetStateById(((struct T2 *)p->pt)->pF64, p->n50, 2);
}
