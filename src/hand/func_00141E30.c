struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; };
/** @sly2-annot addr=0x141E30
 * Purpose: vt104: fire Splice event 0x1A on itself via vt3 HandleSpliceEvent
 * Info: proposed name STEPPLAYER_VUnk104 (T2); class STEPPLAYER slot104; TU stepplayer
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00141E30(struct O *p)
{
    struct VTE *e = &p->vt[3];
    ((void (*)(void *, int, int, int, int, int))e->pfn)((char *)p + e->d, 0x1A, 0, 0, 0, 0);
}
