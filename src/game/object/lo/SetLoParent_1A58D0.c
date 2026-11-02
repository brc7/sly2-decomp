struct VTE { short d; short pad; void (*pfn)(void *, int, int, int); };
struct O { char pad[0xC]; struct VTE *vt; };
/** @sly2-annot addr=0x1A58D0
 * Purpose: Non-virtual wrapper: vt10(paloParent, 0, b)
 * Info: proposed name SetLoParent_1A58D0 (T1); TU lo
 * Notes: COLLISION: SetLoParent already used (same name proposed for 0x1A5900); renamed to SetLoParent_1A58D0
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void SetLoParent_1A58D0(struct O *p, int a, int b) { struct VTE *e = &p->vt[10]; e->pfn((char *)p + e->d, a, 0, b); }
