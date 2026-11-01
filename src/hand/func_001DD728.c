struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a, b, c; struct VTE *vt; };
struct S { int a, b, c; struct VTE *vt; char pad[0x14]; struct O *po; };
/** @sly2-annot addr=0x1DD728
 * Purpose: Hide screen 0x52D (vt11) and its widget (vt50)
 * Info: proposed name HideScreen52D (T2); TU screen
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DD728(struct S *p) {
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
