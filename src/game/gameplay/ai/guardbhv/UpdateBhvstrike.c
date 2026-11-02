typedef int qword __attribute__((mode(TI)));
struct T { int a, b; void *p8; };
struct S { char pad[0x38]; void *p38; int pad2; struct T *p40; char pad3[0xC]; int n50; };
void UpdateBhvanim(struct S *p);
qword func_001A62E8(void *p);
void func_0020C6F8(void *p, qword q);
/** @sly2-annot addr=0x175258
 * Purpose: BHVSTRIKE Update: BHVANIM update; while leap enabled keep the leap target on the victim
 * Info: proposed name UpdateBhvstrike (T1); class BHVSTRIKE slot10; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVSTRIKE role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void UpdateBhvstrike(struct S *p) { UpdateBhvanim(p); if (!p->p40 || !p->p40->p8) p->n50 = 0; if (p->n50) { struct T *t = p->p40; func_0020C6F8(t->p8, func_001A62E8(p->p38)); } }
