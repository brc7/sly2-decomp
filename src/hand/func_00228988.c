struct S { char pad[0x16C]; int c; int a[4]; };
/** @sly2-annot addr=0x228988
 * Purpose: if c<4 a[c++]=v
 * Info: proposed name AddWarpOid (T2); TU exit_warp
 * Notes: descriptive only: Add oid to 4-slot list | no usable identifier; purpose only | arbitration knock-on
 *   rename: CONFIRMED static: WARP 4-entry OID list (+0x16C count, +0x170); thunk optid 20 on WARP;
 *   0x2289B0/0x2289D0 add fixed OIDs 0x189/0x18A.
 * Confidence: low (draft) [T2 0.45] - evidence: arbitration; sources: r8_21BB88+arbitration
 * See: docs/engine/world-sw.md#10-key-functions,
 *   docs/reference/functions/world-sw.md#exit_warp-0x227fe8-0x228b48
 * @end-sly2-annot */
void func_00228988(struct S *p, int v)
{
    if ((unsigned)p->c < 4) p->a[p->c++] = v;
}
