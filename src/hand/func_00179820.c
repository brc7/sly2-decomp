struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; };
struct T { char pad[0xFEC]; struct O *pFEC; };
struct S { char pad[0x10]; struct T *pt; };
void CloseBhvhit(struct S *p);
/** @sly2-annot addr=0x179820
 * Purpose: BHVKNOCKDOWN Close: BHVHIT Close and close helper +0xFEC
 * Info: proposed name CloseBhv162 (T2); class BHV162 slot9; TU guardbhv
 * Notes: BHV class word normalised: CloseBhvknockdown -> CloseBhv162
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_00179820(struct S *p) { struct O *o; CloseBhvhit(p); o = p->pt->pFEC; if (o) { struct VTE *e = &o->vt[9]; e->pfn((char *)o + e->d); } }
