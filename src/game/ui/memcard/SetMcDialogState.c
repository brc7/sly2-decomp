struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x14]; struct O *x24; char pad3[0x18]; int x40; };
/** @sly2-annot addr=0x1ACB58
 * Purpose: x40 = state; if shown (x24) call vt10 and child vt49(-1)
 * Info: proposed name SetMcDialogState (T1); TU memcard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/save-memcard.md#5-key-functions, docs/engine/save-memcard.md#open-questions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void SetMcDialogState(struct O *p, int a)
{
    p->x40 = a;
    if (p->x24) {
        { struct VTE *e = &p->vt[10]; e->pfn((char *)p + e->d); }
        { struct O *o = p->x24; struct VTE *e = &o->vt[49]; e->pfn((char *)o + e->d, -1); }
    }
}
