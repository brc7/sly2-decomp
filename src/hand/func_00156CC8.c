void InitXfm(void *p);
void SetLoBusyk(void *p, int n);
/** @sly2-annot addr=0x156CC8
 * Purpose: XFM::Init then 0x1A6CA8(p,0), flag 2
 * Info: proposed name InitCid47 (T2); class CID47 slot7; TU noise
 * Notes: Cid2F->Cid47 (CID 0x2F=47, hex-literal)
 * Confidence: low (draft) [T2 0.75] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/guards-ai.md#noise-0x1564e8-0x157200
 * @end-sly2-annot */
void func_00156CC8(char *p)
{
    unsigned int v;
    InitXfm(p);
    SetLoBusyk(p, 0);
    v = *(unsigned int *)(p + 0x40);
    if ((v ^ 1) & 1) *(int *)v |= 2;
    else *(unsigned int *)(p + 0x40) = v | 2;
}
