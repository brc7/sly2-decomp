struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; };
/** @sly2-annot addr=0x1A5D08
 * Purpose: Callback body: vt9 Remove(plo)
 * Info: proposed name DeferLoRemoveCallback (T1); TU lo
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void DeferLoRemoveCallback(struct O *p) { struct VTE *e = &p->vt[9]; e->pfn((char *)p + e->d); }
