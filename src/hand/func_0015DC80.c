void InitLo(void *p);
void SetLoBusyk(void *p, int n);
/** @sly2-annot addr=0x15DC80
 * Purpose: LO::Init, register for update, flags 0xA
 * Info: proposed name InitCid116 (T2); class CID116 slot7; TU tu_15DC80
 * Notes: Cid74->Cid116 (CID 0x74=116, vtable)
 * Confidence: low (draft) [T2 0.75] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/gameplay-objects.md#source-files, docs/engine/hud-ui.md#4-source-files,
 *   docs/reference/functions/gameplay-objects.md#tu_15dc80-0x15dc80-0x15e380
 * @end-sly2-annot */
void func_0015DC80(char *p)
{
    unsigned int v;
    InitLo(p);
    SetLoBusyk(p, 1);
    v = *(unsigned int *)(p + 0x40);
    if ((v ^ 1) & 1) *(int *)v |= 0xA;
    else *(unsigned int *)(p + 0x40) = v | 0xA;
}
