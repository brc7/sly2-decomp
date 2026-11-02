struct O { char pad[0x58]; int n58; char p1[8]; int n64; };
struct S { char pad[0x24]; struct O *po; };
/** @sly2-annot addr=0x1DD780
 * Purpose: Widget fully shown (+0x58==2 and +0x64==2)
 * Info: proposed name FScreen52DShown (T2); TU screen
 * Notes: verifier PLAUSIBLE: '52D' is an opaque placeholder. Returns widget(+0x24)->+0x58 == 2 && +0x64 == 2;
 *   only caller UpdateCd (disc-check state 1) on screen D_003DA4F0. | T2 forced: verifier: placeholder name
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r9_bhv_screen+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
int func_001DD780(struct S *p) {
    struct O *po = p->po;
    if (!po) return 0;
    if (po->n58 != 2) return 0;
    return po->n64 == 2;
}
