struct E { int a; int b; };
struct T { int a, b; struct E *ae; };
struct S { char pad[0x39E8]; struct T *pt; };
/** @sly2-annot addr=0x1FFA40
 * Purpose: Splice table entry n -> frame (NULL for -1)
 * Info: proposed name PframeFromIsplice (T1); TU splicemap
 * Notes: splice_scripts format doc cites this function by this name (CONFIRMED(code) in the doc)
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r7_1F8DD8+format-doc:splice_scripts
 * See: docs/engine/splice-scripting.md#1-layout-of-the-runtime,
 *   docs/formats/splice-scripts.md#1-where-the-scripts-live-cstatic--cparse,
 *   docs/formats/world-file.md#4-per-world-entry-data-after-the-head
 * @end-sly2-annot */
int func_001FFA40(struct S *p, int n) { if (n == -1) return 0; return p->pt->ae[n].b; }
