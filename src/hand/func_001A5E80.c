struct T { unsigned int x0; };
struct O;
struct S { char pad[0x10]; struct O *x10; char pad2[0x2C]; unsigned int x40; };
struct O { char pad[0x40]; struct T *x40; };
static inline void ClearFlag(struct S *p, unsigned int fl)
{
    unsigned int v = p->x40;
    if ((v ^ 1) & 1) *(unsigned int *)v &= ~fl;
    else p->x40 = v & ~fl;
}
/** @sly2-annot addr=0x1A5E80
 * Purpose: LO vt17: clear grf bit 4 unless parent grf has bit 4
 * Info: proposed name LO_vt17_InheritFlag4 (T2); class LO slot17; TU lo
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/object-model.md#2-the-lo-virtual-interface,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void func_001A5E80(struct S *p)
{
    if (!p->x10 || !(p->x10->x40->x0 & 4)) ClearFlag(p, 4);
}
