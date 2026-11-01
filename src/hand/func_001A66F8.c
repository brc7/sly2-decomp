struct S { char pad[0x40]; unsigned int x40; };
/** @sly2-annot addr=0x1A66F8
 * Purpose: grf(x40) &= ~fl (XFM vt48)
 * Info: proposed name ClearLoGrf (T2); class LO slot48; TU lo
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/physics-collision.md#1-so-the-solid-object-socpp-0x1f2108-0x1f8dd8-174-functions,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void func_001A66F8(struct S *p, unsigned int fl)
{
    unsigned int v = p->x40;
    if ((v ^ 1) & 1) *(unsigned int *)v &= ~fl;
    else p->x40 = v & ~fl;
}
