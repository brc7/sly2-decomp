struct F { int a[0x1C]; };
void *func_00205458(void *p, int oid);
void *func_002333B4(void *pv, int c, int cb);
void SetBhvguardMode(void *p, struct F *pf);
/** @sly2-annot addr=0x209E28
 * Purpose: Send record {k=6, a[4]=n} to root behaviour BHV156 (0x176750)
 * Info: proposed name SendStepguardRootBhv6 (T2); TU stepguard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00209E28(void *p, int n) { void *o = func_00205458(p, 0x9C); struct F f; func_002333B4(&f, 0, sizeof f); f.a[0] = 6; f.a[4] = n; SetBhvguardMode(o, &f); }
