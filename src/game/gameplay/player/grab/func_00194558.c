struct O { char pad[0x12F0]; int x12f0; };
struct S { struct O *p; int a; int k; };
extern int D_002DFBC0;
extern char D_002DE2E0[];
int FActiveCplcy__FP5CPLCY(void *);
static __inline__ int FX(struct S *p) {
    int r = 0;
    if (p->p->x12f0 && (D_002DFBC0 & 2)) r = !FActiveCplcy__FP5CPLCY(D_002DE2E0);
    return r;
}
/** @sly2-annot addr=0x194558
 * Purpose: Can the player use gadget/action k while in this grab kind (k==3 special; kinds 1/8/11/12 forbid;
 *   3/4 allow 1,2,0x1F)
 * Info: proposed name FJtGrabAllowsGadget (T2); TU grab
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grab-0x191090-0x195950
 * @end-sly2-annot */
int func_00194558(struct S *p, int k) {
    if (k == 3) {
        switch (p->k) {
        case 0: case 1: case 3: case 4: case 13:
            return FX(p);
        }
        return 0;
    }
    switch (p->k) {
    case 1: case 8: case 11: case 12:
        return 0;
    case 3: case 4: {
        int f = 0;
        if (k == 1 || k == 2 || k == 0x1F) f = 1;
        return f;
    }
    }
    return 1;
}
