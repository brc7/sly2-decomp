void func_00231100(void *p);
void func_00148718(void *p);
void func_001A6CA8(void *p, int n);
/** @sly2-annot addr=0x1492F0
 * Purpose: cid 0x18/0x19 vt7: base init, InitBtn
 * Info: proposed name InitVolbtn (T1); class VOLBTN slot7; TU button
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void func_001492F0(char *p)
{
    unsigned int v;
    func_00231100(p);
    func_00148718(p + 0x130);
    *(int *)(p + 0x1CC) = 1;
    func_001A6CA8(p, 0);
    v = *(unsigned int *)(p + 0x40);
    if ((v ^ 1) & 1) *(int *)v |= 2;
    else *(unsigned int *)(p + 0x40) = v | 2;
}
