struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; };
struct S { struct O *po; };
/** @sly2-annot addr=0x1A30D0
 * Purpose: Call the letterbox object's vt50 (hide)
 * Info: proposed name HideLetterbox (T1); TU letterbox
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/world-sw.md#10-key-functions,
 *   docs/reference/functions/hud-ui.md#letterbox-0x1a3020-0x1a3260,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void HideLetterbox(struct S *p) { struct O *o = p->po; if (o) { struct VTE *e = &o->vt[50]; e->pfn((char *)o + e->d); } }
