typedef unsigned long ulong;
struct VTE { short d; short pad; void (*pfn)(void *, int, void *); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x18]; ulong fl; };
/** @sly2-annot addr=0x1A5800
 * Purpose: LO vt11: set in-world bit60, then vt4(0,plo) notify (MSGID added)
 * Info: proposed name AddLoHierarchy (T1); class LO slot11; TU lo
 * Notes: names_v2 v1 proposed SetLightDiffuse_Old__FP5LIGHTf (0.001)
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources: r4_17F090
 * See: docs/engine/object-model.md#2-the-lo-virtual-interface,
 *   docs/reference/functions/camera.md#tu_12e600-0x12e600-0x12eb48,
 *   docs/reference/functions/guards-ai.md#noise-0x1564e8-0x157200
 * @end-sly2-annot */
void func_001A5800(struct O *p)
{
    struct VTE *e;
    p->fl |= (ulong)1 << 60;
    e = &p->vt[4];
    e->pfn((char *)p + e->d, 0, p);
}
