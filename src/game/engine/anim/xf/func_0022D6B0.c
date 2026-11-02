struct S { char pad[0x54]; int base; };
extern int D_004437A0;
extern int D_004437A8[];
/** @sly2-annot addr=0x22D6B0
 * Purpose: id&0x3000: 0 local table, 0x1000 parent table, 0x2000 context stack top
 * Info: proposed name PxfResolveId (T2); TU xf
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r8_21BB88
 * See: docs/engine/animation.md#4-xf-slots-and-transforms-xfcpp-0x22c818-lilly-xfc,
 *   docs/formats/meshes-scene.md#3-transforms-cstatic,
 *   docs/reference/functions/animation.md#xf-0x22c818-0x231100
 * @end-sly2-annot */
int func_0022D6B0(int a, struct S *b, struct S *c)
{
    int r = 0;
    switch (a & 0x3000) {
    case 0: if (c) r = c->base + (a << 8); break;
    case 0x1000: a &= ~0xF000; r = b->base + (a << 8); break;
    case 0x2000: r = D_004437A8[D_004437A0 - 1]; break;
    }
    return r;
}
