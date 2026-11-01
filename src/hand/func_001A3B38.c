struct G { char pad[0x3D5C]; int x3d5c; };
extern struct G D_003DD490;
extern int D_003E19E4;
struct S { char pad[0xE0]; int xe0; char pad2[0x78]; int x15c; int x160; };
/** @sly2-annot addr=0x1A3B38
 * Purpose: If not dynamic bump g_sw static-light stamp (+0x3D5C); if x15C==0 and kind!=0 set global dirty
 *   D_003E19E4
 * Info: proposed name InvalidateLight (T2); TU light
 * Notes: names_v2 C proposed SetLightFallOff__FP5LIGHTP2LM (0.638)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
void func_001A3B38(struct S *p)
{
    if (!p->x160) D_003DD490.x3d5c++;
    if (!p->x15c && p->xe0) D_003E19E4 = 1;
}
