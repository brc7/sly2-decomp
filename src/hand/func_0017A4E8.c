typedef int qword __attribute__((mode(TI)));
struct G { int a; };
extern struct G D_003DD490;
struct T { char pad[0xF64]; void *pF64; };
struct S { char pad[0x10]; struct T *pt; };
void StartBhvanim(struct S *p);
void func_0017A5B0(struct S *p, int n);
void func_00153FF8(void *p);
qword func_001A62E8(void *p);
void func_00156C80(struct G *g, int a, qword q, void *b, void *pv);
/** @sly2-annot addr=0x17A4E8
 * Purpose: BHVSLEEP Start: BHVANIM Start, sub-state 0 (SM goal 0x44, random duration +0xE7C..E80), reset
 *   detector, hold a noise
 * Info: proposed name StartBhv175 (T2); class BHV175 slot8; TU guardbhv
 * Notes: BHV class word normalised: StartBhvsleep -> StartBhv175
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017A4E8(struct S *p) { StartBhvanim(p); func_0017A5B0(p, 0); func_00153FF8(p->pt->pF64); func_00156C80(&D_003DD490, 1, func_001A62E8(p->pt), p->pt, (char *)p->pt + 0x35C); }
