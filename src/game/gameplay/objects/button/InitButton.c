void InitSo(void *p);
void InitBtn(void *p);
/** @sly2-annot addr=0x148BB0
 * Purpose: cid 0x53 vt7: InitSo, InitBtn(+0x150), flag
 * Info: proposed name InitButton (T1); class BUTTON slot7; TU button
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void InitButton(char *p)
{
    unsigned int v;
    InitSo(p);
    InitBtn(p + 0x150);
    v = *(unsigned int *)(p + 0x40);
    if ((v ^ 1) & 1) *(int *)v |= 2;
    else *(unsigned int *)(p + 0x40) = v | 2;
}
