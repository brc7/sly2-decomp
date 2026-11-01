struct S { char pad[0xB0]; int f; };
void func_001A5270(struct S *p);
/** @sly2-annot addr=0x211E40
 * Purpose: CID 0x80 Init: InitLo, +0xB0 = 1
 * Info: proposed name InitWidget128 (T2); class WIDGET128 slot7; TU hud
 * Notes: names_v2 v1 proposed OnBlotReset__FP4BLOT (0.053) | v2: demoted to T2 (placeholder token in name:
 *   128)
 * Confidence: low (draft) [T2 0.60] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00211E40(struct S *p)
{
    func_001A5270(p);
    p->f = 1;
}
