struct S { char pad[0x40]; unsigned int x40; };
/** @sly2-annot addr=0x1A66C8
 * Purpose: LO vt47: grf(x40 tagged word) |= fl
 * Info: proposed name SetLoGrf (T2); class LO slot47; TU lo
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/object-model.md#2-the-lo-virtual-interface,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void func_001A66C8(struct S *p, unsigned int fl)
{
    unsigned int v = p->x40;
    if ((v ^ 1) & 1) *(unsigned int *)v |= fl;
    else p->x40 = v | fl;
}
