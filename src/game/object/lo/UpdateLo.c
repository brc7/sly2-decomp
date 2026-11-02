struct VTE { short d; short pad; void (*pfn)(void *, int, int, int, int, int); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x20]; float x30; };
extern float D_002DDED0;
/** @sly2-annot addr=0x1A5E28
 * Purpose: LO vt16 Update: if tSplice(x30) <= g_clock.t send vt3 message 1 (Splice update event)
 * Info: proposed name UpdateLo (T1); class LO slot16; TU lo
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/object-model.md#2-the-lo-virtual-interface,
 *   docs/reference/functions/guards-ai.md#detector-0x153d58-0x154a60,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void UpdateLo(struct O *p)
{
    if (p->x30 <= D_002DDED0) {
        struct VTE *e = &p->vt[3];
        e->pfn((char *)p + e->d, 1, 1, 0, 0, 0);
    }
}
