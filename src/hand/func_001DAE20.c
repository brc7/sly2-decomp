struct E { int id; int a[7]; };
struct S { char pad[0x4C]; int c; struct E *ae; };
/** @sly2-annot addr=0x1DAE20
 * Purpose: Find an entry (0x20 bytes) by id in the screen's item list (+0x4C/+0x50)
 * Info: proposed name PentryFindScreen (T2); TU screen
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
struct E *func_001DAE20(struct S *p, int id) {
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if (pe->id == id) return pe;
    }
    return 0;
}
