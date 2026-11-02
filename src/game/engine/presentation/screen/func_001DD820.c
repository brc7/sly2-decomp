struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a, b, c; struct VTE *vt; };
struct S { int a, b, c; struct VTE *vt; char pad[0x14]; struct O *po; };
void func_001D5498(void);
/** @sly2-annot addr=0x1DD820
 * Purpose: Show the title screen (D_003DA530, widget 0x5D2) and its widget; reset rumble
 * Info: proposed name ShowTitleScreen (T2); TU screen
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001DD820(struct S *p) {
    if (p->po) {
        {
            struct VTE *e = &p->vt[10];
            ((void (*)(void *))e->pfn)((char *)p + e->d);
        }
        {
            struct O *po = p->po;
            struct VTE *e = &po->vt[49];
            ((void (*)(void *, int))e->pfn)((char *)po + e->d, -1);
        }
        func_001D5498();
    }
}
