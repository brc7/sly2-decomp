struct VTE { short d; short pad; void (*pfn)(void *, int); };
struct O { int a[3]; struct VTE *vt; };
void func_001CB5D0(struct O *p);
/** @sly2-annot addr=0x1C7EA8
 * Purpose: CID 95 vt27
 * Info: proposed name ClueVt27 (T2); class CLUE slot27; TU prize
 * Notes: verifier: purpose partly wrong: CID 95 slot 27 (proxy-family virtual, base SO slot 27 0x1CB5D0) then
 *   SetDprizeDprizes(-1). Placeholder, 'Clue' unproven; purpose does not describe behaviour.
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void func_001C7EA8(struct O *p)
{
    func_001CB5D0(p);
    { struct VTE *e = &p->vt[76]; e->pfn((char *)p + e->d, -1); }
}
