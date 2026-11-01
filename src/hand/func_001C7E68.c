struct VTE { short d; short pad; void (*pfn)(void *, int); };
struct O { int a[3]; struct VTE *vt; };
void func_001CB518(struct O *p);
/** @sly2-annot addr=0x1C7E68
 * Purpose: CID 95 vt26: SO::VUnk26Proxy base + extra
 * Info: proposed name ClueVt26 (T2); class CLUE slot26; TU prize
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void func_001C7E68(struct O *p)
{
    func_001CB518(p);
    { struct VTE *e = &p->vt[76]; e->pfn((char *)p + e->d, 0); }
}
