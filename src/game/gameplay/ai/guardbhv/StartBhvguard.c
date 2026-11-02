struct F { int a[0x1C]; };
void StartBhv(void *p);
void func_002333B4(void *p, int c, int n);
void SetBhvguardMode(void *p, struct F *pf);
/** @sly2-annot addr=0x176708
 * Purpose: BHVGUARD Start: enter mode 0 (immediately becomes patrol)
 * Info: proposed name StartBhvguard (T1); class BHVGUARD slot8; TU guardbhv
 * Notes: names_v2 C hint not used: FIsZeroDv__FP6VECTOR | runtime2/4 confirmed the BHVGUARD role on live
 *   guards (S02-S41)
 * Confidence: mid (draft) [T1 0.75] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void StartBhvguard(void *p) { struct F f; StartBhv(p); func_002333B4(&f, 0, sizeof(f)); f.a[0] = 0; SetBhvguardMode(p, &f); }
