struct VTE { short d; short pad; void (*pfn)(void *, int); };
struct O { char pad[0xC]; struct VTE *vt; };
/** @sly2-annot addr=0x1A6AD0
 * Purpose: LO vt40: forwards to vt41 Clone
 * Info: proposed name CloneLoHierarchy (T1); class LO slot40; TU lo
 * Notes: runtime: LO slot 40 = CloneLoHierarchy 0x1A6AD0 confirmed
 * Confidence: high [T1 0.85] - evidence: runtime (save-state RAM); sources: r4_17F090+runtime
 * See: docs/engine/object-model.md#2-the-lo-virtual-interface, docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void func_001A6AD0(struct O *p, int a) { struct VTE *e = &p->vt[41]; e->pfn((char *)p + e->d, a); }
