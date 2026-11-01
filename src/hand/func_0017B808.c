struct S { char pad[0x10]; void *p10; char pad2[0x1C]; int n30; };
void func_001EEF68(struct S *p);
void func_00209890(void *p, int n);
/** @sly2-annot addr=0x17B808
 * Purpose: BHVTARGETS Init: raise flag 0x20, state -1
 * Info: proposed name InitBhv178 (T2); class BHV178 slot7; TU guardbhv
 * Notes: BHV class word normalised: InitBhvtargets -> InitBhv178
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017B808(struct S *p) { func_001EEF68(p); func_00209890(p->p10, 0x20); p->n30 = -1; }
