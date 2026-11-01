typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0;
struct G { int a; };
extern struct G D_003DD490;
struct S { char pad[0x10]; char *p10; };
void CloseBhvanim(struct S *p);
void func_0017A5B0(struct S *p, int n);
void func_00156C80(struct G *g, int a, qword q, int b, void *pv);
/** @sly2-annot addr=0x17A558
 * Purpose: BHVSLEEP Close: BHVANIM Close, release held noise
 * Info: proposed name CloseBhv175 (T2); class BHV175 slot9; TU guardbhv
 * Notes: BHV class word normalised: CloseBhvsleep -> CloseBhv175
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017A558(struct S *p) { CloseBhvanim(p); func_0017A5B0(p, -1); func_00156C80(&D_003DD490, -1, D_002A2DD0, 0, p->p10 + 0x35C); }
