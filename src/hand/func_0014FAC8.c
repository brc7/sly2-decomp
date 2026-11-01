void func_00205EB8(void *p);
/** @sly2-annot addr=0x14FAC8
 * Purpose: vt7 Init override of guard class cid 0x40: base STEPGUARD Init (0x205EB8) then +0x4A4 = 1
 * Info: proposed name InitContessa (T2); class CONTESSA slot7; TU tu_14FAC8
 * Notes: class identity from whole_game WE names/object strings: InitCid64Guard -> InitContessa
 * Confidence: low (draft) [T2 0.45] - evidence: semantic review; sources: r2_131F70+whole_game(class
 *   identity)
 * See: docs/engine/camera.md#1-cm-the-camera-singleton, docs/engine/camera.md#4-source-files,
 *   docs/engine/guards-ai.md#8-source-files
 * @end-sly2-annot */
void func_0014FAC8(char *p)
{
    func_00205EB8(p);
    *(int *)(p + 0x4A4) = 1;
}
