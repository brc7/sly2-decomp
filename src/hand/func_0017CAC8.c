struct S { char pad[0x10]; void *p10; char pad2[0x1C]; int n30; int pad3; float g38; };
void func_001EEF68(struct S *p);
void func_00209890(void *p, int n);
/** @sly2-annot addr=0x17CAC8
 * Purpose: BHVRAIL Init: raise flag 0x1B (on rail), state -1
 * Info: proposed name InitBhvrail (T1); class BHVRAIL slot7; TU guardbhv
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017CAC8(struct S *p) { func_001EEF68(p); p->n30 = -1; p->g38 = 1.0f; func_00209890(p->p10, 0x1B); }
