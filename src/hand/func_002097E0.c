struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { int a, b, c; struct VTE *vt; };
/** @sly2-annot addr=0x2097E0
 * Purpose: call driver vt11
 * Info: proposed name UpdateStepguardMover (T2); TU stepguard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_002097E0(struct O **pp) { struct O *o = *pp; if (o) { struct VTE *e = &o->vt[11]; e->pfn((char *)o + e->d); } }
