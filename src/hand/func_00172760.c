typedef int qword __attribute__((mode(TI)));
struct T { char pad[0x20]; qword q20; qword q30; char pad2[0x4]; int n44; char pad3[0xC]; int n54; };
struct S { char pad[0x10]; int n10; char pad2[0x30]; struct T *pt; };
/** @sly2-annot addr=0x172760
 * Purpose: Last-known target position from the perception record (q20 if mode 4 for this guard, else q30)
 * Info: proposed name PosBhv171LastKnown (T2); TU tu_171F58
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_171f58-0x171f58-0x173158
 * @end-sly2-annot */
qword func_00172760(struct S *p) { struct T *t = p->pt; if (t->n54 == 4 && t->n44 == p->n10) return t->q20; return p->pt->q30; }
