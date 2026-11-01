struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x2C]; int n3C; };
struct S { char pad[0x10]; void *p10; };
void StartBhv(struct S *p);
struct O *func_00205438(void *p, int n);
/** @sly2-annot addr=0x17D2E0
 * Purpose: BHVATTACH39 Start: push BHVANIM flag 2
 * Info: proposed name StartBhv188 (T2); class BHV188 slot8; TU guardbhv
 * Notes: BHV class word normalised: StartBhvattach39 -> StartBhv188
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017D2E0(struct S *p) { struct O *o; struct VTE *e; StartBhv(p); o = func_00205438(p->p10, 0x9D); o->n3C = 2; e = &o->vt[8]; e->pfn((char *)o + e->d); }
