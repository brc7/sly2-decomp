typedef struct { float m[4][4]; } __attribute__((aligned(16))) M4;
typedef struct Y { char pad[0x40]; M4 m40; char pad2[0x24]; int xA4; } Y;
typedef struct X { char pad[0x50]; Y *y; int x54; int pad2[2]; M4 m60; M4 mA0; } X;
M4 func_0011C168(M4 *, M4 *);

/** @sly2-annot addr=0x231368
 * Purpose: recompute matWorld when parent stamp newer
 * Info: proposed name UpdateXfmMatWorld (T1); TU xform
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/object-model.md#xform-0x231100-0x2319e8
 * @end-sly2-annot */
void func_00231368(X *p)
{
    Y *y = p->y;
    if (y) {
        if (p->x54 < y->xA4) {
            p->x54 = y->xA4;
            p->mA0 = func_0011C168(&y->m40, &p->m60);
        }
    } else if (p->x54 < 0) {
        p->mA0 = p->m60;
    }
}
