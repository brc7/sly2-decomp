struct F { int a0; int pad; int n8; int nC; int n10; int pad2[0x13]; };
struct S { char pad[0x10]; int n10; };
void *func_002333B4(void *p, int c, int n);
void func_001F7598(struct F *pf);
/** @sly2-annot addr=0x176B80
 * Purpose: Inflict a zap of kind 0x40000 and amount a on the guard from source b
 * Info: proposed name InflictBhvguardZap (T1); TU guardbhv
 * Notes: names_v2 C hint not used: FIsZeroDw__FP6VECTOR | runtime2/4 confirmed the BHVGUARD role on live
 *   guards (S02-S41)
 * Confidence: mid (draft) [T1 0.50] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_00176B80(struct S *p, int a, int b) { struct F f; func_002333B4(&f, 0, sizeof(f)); f.a0 = b; f.n10 = a; f.nC = 0x40000; f.n8 = p->n10; func_001F7598(&f); }
