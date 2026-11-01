struct VTE { short d; short pad; void (*pfn)(void *, int, void *); };
struct VT { struct VTE e[8]; };
struct S { int x0, x4, x8; struct VT *vt; char pad[0x18]; unsigned long x28; };
extern char D_003DD490[];
void func_001A00D0(void *g, struct S *s, int n);
/** @sly2-annot addr=0x1A5848
 * Purpose: LO vt12: clear bit60; if bit59 clear references via 0x1A00D0(g_sw); vt4(1,plo) notify (MSGID
 *   removed)
 * Info: proposed name RemoveLoHierarchy (T1); class LO slot12; TU lo
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/object-model.md#2-the-lo-virtual-interface,
 *   docs/reference/functions/camera.md#tu_12e600-0x12e600-0x12eb48,
 *   docs/reference/functions/guards-ai.md#noise-0x1564e8-0x157200
 * @end-sly2-annot */
void func_001A5848(struct S *s)
{
    s->x28 &= ~0x1000000000000000UL;
    if (s->x28 & 0x0800000000000000UL)
        func_001A00D0(D_003DD490, s, 0);
    s->vt->e[4].pfn((char *)s + s->vt->e[4].d, 1, s);
}
