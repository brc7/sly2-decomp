void InitXfm(void *p);
void SetLoBusyk(void *p, int n);
/** @sly2-annot addr=0x153D58
 * Purpose: XFM::Init (0x231100), registers for update (0x1A6CA8(p,1)), sets flag 2
 * Info: proposed name InitDetector (T1); class DETECTOR slot7; TU detector
 * Notes: verifier PLAUSIBLE: InitCid10 -> InitDetector ?
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r3_1539A8+verdicts_tu_r3r5
 * See: docs/engine/gameplay-objects.md#source-files, docs/engine/guards-ai.md#8-source-files,
 *   docs/engine/guards-ai.md#9-key-functions
 * @end-sly2-annot */
void InitDetector(char *p)
{
    unsigned int v;
    InitXfm(p);
    SetLoBusyk(p, 1);
    v = *(unsigned int *)(p + 0x40);
    if ((v ^ 1) & 1) *(int *)v |= 2;
    else *(unsigned int *)(p + 0x40) = v | 2;
}
