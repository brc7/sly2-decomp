struct VTE { short d; short pad; void (*pf)(void *, int); };
struct S { int a[3]; struct VTE *vt; char pad[0x14C]; int k; };
/** @sly2-annot addr=0x2245B8
 * Purpose: Fire a held/stuck Bentley gadget object: Health Extractor (id 11) -> SetCid100State(5), else
 *   (Trigger Bomb) -> state 3. Caller HandleStepplayerMessage msg 9 kind 0x27C (2nd gadget press, JTGRAB
 *   nState 0xB)
 * Info: proposed name TriggerCid100 (T2); TU tu_223F70
 * Notes: descriptive only: Cid100 release state | no usable identifier; purpose only | runtime5a #17: S51
 *   JTGRAB tEnter 571.17, bomb state 3 at 571.57 | T2: placeholder token Cid100
 * Confidence: low (draft) [T2 0.60] - evidence: runtime (save-state RAM); sources: r8_21BB88+runtime5a
 * See: docs/engine/player.md#7-bentley-bentleycpp-0x134648
 * @end-sly2-annot */
void func_002245B8(struct S *p)
{
    if (p->k == 11) p->vt[76].pf((char *)p + p->vt[76].d, 5);
    else p->vt[76].pf((char *)p + p->vt[76].d, 3);
}
