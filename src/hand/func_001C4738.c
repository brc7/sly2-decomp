struct S { char pad[0x88]; float a[32]; };
/** @sly2-annot addr=0x1C4738
 * Purpose: Set atPressed[i] = -FLT_MAX for every bit in grf
 * Info: proposed name ClearPoctlJoyPressed (T1); class POCTL; TU po
 * Notes: names_v2 B proposed LoadRostFromBrx__FP4ROSTP18CBinaryInputStream (0.965) | mangled dropped (base
 *   does not match final name): ClearJoyPressed__5POCTLi
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
void func_001C4738(struct S *p, int grf)
{
    float *pg;
    if (!grf) return;
    pg = p->a;
    do { if (grf & 1) *pg = -3.40282346638528859812e+38f; grf >>= 1; pg++; } while (grf);
}
