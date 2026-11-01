struct S { char pad[0x40]; unsigned int x40; };
static inline void SetFlag(struct S *p, unsigned int fl)
{
    unsigned int v = p->x40;
    if ((v ^ 1) & 1) *(unsigned int *)v |= fl;
    else p->x40 = v | fl;
}
static inline void ClearFlag(struct S *p, unsigned int fl)
{
    unsigned int v = p->x40;
    if ((v ^ 1) & 1) *(unsigned int *)v &= ~fl;
    else p->x40 = v & ~fl;
}
/** @sly2-annot addr=0x1A6E80
 * Purpose: LO vt21(f): set/clear grf bit 8
 * Info: proposed name SetLoFlag8 (T2); class LO slot21; TU lo
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void func_001A6E80(struct S *p, int f)
{
    if (f) SetFlag(p, 8);
    else ClearFlag(p, 8);
}
