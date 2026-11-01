struct F { int a[0x1C]; };
void *func_00205458(void *p, int n);
void *func_002333B4(void *p, int c, int n);
void SetBhvguardMode(void *p, struct F *pf);
/** @sly2-annot addr=0x171D58
 * Purpose: Sets the guard's BHV156 to mode 1 (probably back to normal routine)
 * Info: proposed name SetGuardBhvguardMode1 (T2); TU tu_1709A8
 * Notes: BHV class word normalised: SetGuardBhv156Mode1 -> SetGuardBhvguardMode1 | runtime2/4 confirmed the
 *   BHVGUARD role on live guards (S02-S41)
 * Confidence: low (draft) [T2 0.35] - evidence: runtime (save-state RAM); sources: r3b_rest+runtime(class
 *   role)
 * See: docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
void func_00171D58(void *p) { struct F f; void *o = func_00205458(p, 0x9C); if (o) { func_002333B4(&f, 0, sizeof(f)); f.a[0] = 1; SetBhvguardMode(o, &f); } }
