void func_001C3D70(void *p);
extern void *D_003D4A54;
/** @sly2-annot addr=0x1D7238
 * Purpose: PO subclass Init: InitPo then store self in global D_003D4A54
 * Info: proposed name InitSafehouse (T2); class SAFEHOUSE slot7; TU tu_1D7238
 * Notes: class identity from whole_game WE names/object strings: InitCharSelectPo -> InitSafehouse
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6_1D5150+whole_game(class
 *   identity)
 * See: docs/engine/physics-collision.md#6-source-files,
 *   docs/engine/player.md#8-look-mode-recon-mode-and-the-ai-partner, docs/engine/player.md#10-source-files
 * @end-sly2-annot */
void func_001D7238(void *p) {
    func_001C3D70(p);
    D_003D4A54 = p;
}
