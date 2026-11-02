struct CRef { int tagk; int tag; };
void func_002013D8(struct CRef *pref, void *p, int a, int b, int *pn, int *an);
unsigned long GrfFromSgfArray(int c, int *a);
void func_00129EC8(struct CRef *pref);
/** @sly2-annot addr=0x207B10
 * Purpose: Convert a Splice list argument of guard-flag enums (type 0x1010) to a mask
 * Info: proposed name GrfFromSpliceSgfList (T2); TU stepguard
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
unsigned long func_00207B10(void *p) { int an[16]; struct CRef ref; int c; unsigned long m; func_002013D8(&ref, p, 0x1010, 0x10, &c, an); m = GrfFromSgfArray(c, an); func_00129EC8(&ref); return m; }
