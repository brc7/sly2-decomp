void func_001F2108(void *p);
void func_001A6CA8(void *p, int n);
/** @sly2-annot addr=0x1471C0
 * Purpose: cid 0x68 (SO, 0x180) vt7: InitSo, state -1, flags
 * Info: proposed name InitCid104 (T2); class CID104 slot7; TU tu_1471C0
 * Notes: Cid68->Cid104 (CID 0x68=104, vtable)
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/cutscenes.md#3-screen-transitions, docs/engine/cutscenes.md#4-source-files,
 *   docs/engine/player.md#10-source-files
 * @end-sly2-annot */
void func_001471C0(char *p)
{
    unsigned int v;
    func_001F2108(p);
    *(int *)(p + 0x150) = -1;
    func_001A6CA8(p, 1);
    v = *(unsigned int *)(p + 0x40);
    if ((v ^ 1) & 1) *(int *)v |= 0x102;
    else *(unsigned int *)(p + 0x40) = v | 0x102;
}
