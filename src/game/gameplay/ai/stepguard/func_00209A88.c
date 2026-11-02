struct CRef { int tagk; int tag; };
struct S { char pad[0x2C0]; long n; };
void *CsgfFromGrf(long n, char *ab);
void func_00200F58(struct CRef *ret, void *p, int n, char *ab);
/** @sly2-annot addr=0x209A88
 * Purpose: Splice getter: list of set flags
 * Info: proposed name RefGetStepguardSgfs (T2); TU stepguard
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
struct CRef *func_00209A88(struct CRef *ret, struct S *p) { char ab[0xC0]; func_00200F58(ret, CsgfFromGrf(p->n, ab), 0x1010, ab); return ret; }
