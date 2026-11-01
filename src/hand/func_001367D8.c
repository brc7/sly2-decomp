struct VTE { short d; short pad; void (*pfn)(); };
struct S { int pad[3]; struct VTE *vt; char pad2[0x208 - 0x10]; int f208; char pad3[0x1800 - 0x20c]; int st; };
void func_00135248(struct S *, int);
/** @sly2-annot addr=0x1367D8
 * Purpose: Small transition loop (state 1 returns to 0 and makes the PO current via vt79)
 * Info: proposed name UpdateBentleyStateMisc (T2); TU bentley
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
void func_001367D8(struct S *p) {
    int st;
    do {
        st = p->st;
        switch (st) {
        case 0: break;
        case 1:
            if (p->f208) {
                func_00135248(p, 0);
                ((void (*)(void *, int))p->vt[79].pfn)((char *)p + p->vt[79].d, 1);
            }
            break;
        }
    } while (p->st != st && p->st != -1);
}
