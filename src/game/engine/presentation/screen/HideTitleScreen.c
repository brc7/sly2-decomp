struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a, b, c; struct VTE *vt; };
struct S { int a, b, c; struct VTE *vt; char pad[0x14]; struct O *po; };
/** @sly2-annot addr=0x1DD888
 * Purpose: Hide the title screen and its widget
 * Info: proposed name HideTitleScreen (T1); TU screen
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void HideTitleScreen(struct S *p) {
    if (p->po) {
        {
            struct VTE *e = &p->vt[11];
            ((void (*)(void *))e->pfn)((char *)p + e->d);
        }
        {
            struct O *po = p->po;
            struct VTE *e = &po->vt[50];
            ((void (*)(void *))e->pfn)((char *)po + e->d);
        }
    }
}
