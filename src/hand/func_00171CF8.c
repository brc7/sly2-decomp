struct F { int a[0x1C]; };
void *func_00205458(void *p, int n);
void *func_002333B4(void *p, int c, int n);
void SetBhvguardMode(void *p, struct F *pf);
/** @sly2-annot addr=0x171CF8
 * Purpose: Sets the guard's BHV156 to mode 5 with the given script
 * Info: proposed name StartGuardScript (T2); TU tu_1709A8
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest
 * See: docs/engine/guards-ai.md#3-the-behaviour-stack-and-the-guard-modes,
 *   docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
void func_00171CF8(void *p, int n) { struct F f; void *o = func_00205458(p, 0x9C); if (o) { func_002333B4(&f, 0, sizeof(f)); f.a[4] = n; f.a[0] = 5; SetBhvguardMode(o, &f); } }
