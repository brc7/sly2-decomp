struct VTE { short d; short pad; int (*pfn)(void *); };
struct O { int a, b, c; struct VTE *vt; };
struct S { int a; struct O *po; };
int func_0020FA10(void *pv, struct S *p);
/** @sly2-annot addr=0x20FA50
 * Purpose: FFilterSoNotSelf && object vt62 predicate
 * Info: proposed name FFilterSoNotSelfVt62 (T2); TU sw
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
int func_0020FA50(void *pv, struct S *p) { struct O *o; struct VTE *e; if (!func_0020FA10(pv, p)) return 0; o = p->po; e = &o->vt[62]; return e->pfn((char *)o + e->d); }
