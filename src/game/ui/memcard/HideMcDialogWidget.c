struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; };
struct S { char pad[0x24]; struct O *po; };
/** @sly2-annot addr=0x1ADE70
 * Purpose: small
 * Info: proposed name HideMcDialogWidget (T1); TU memcard
 * Notes: verifier PLAUSIBLE: (McStatus show/hide) -> HideMcDialogWidget
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void HideMcDialogWidget(struct S *p) { struct O *o = p->po; if (o) { struct VTE *e = &o->vt[50]; e->pfn((char *)o + e->d); } }
