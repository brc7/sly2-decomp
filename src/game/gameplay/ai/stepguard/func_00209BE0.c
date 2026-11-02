struct E { char pad[0x10]; long id; char pad2[8]; };
struct S { char pad[0x4C0]; int c; char pad2[0xC]; struct E a[1]; };
/** @sly2-annot addr=0x209BE0
 * Purpose: Binding with exactly this flag set
 * Info: proposed name PanimbindFind (T2); TU stepguard
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
struct E *func_00209BE0(struct S *p, long id) { int i; for (i = 0; i < p->c; i++) { struct E *pe = &p->a[i]; if (pe->id == id) return pe; } return 0; }
