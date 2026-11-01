struct G { int a[4]; };
extern struct G D_003DD4A0;
/** @sly2-annot addr=0x1A5448
 * Purpose: (LO *ploParent, DL **ppdl, LO ***pploRemovedFirst): parent? &parent->dlChild(+0x64),&parent+0x70 :
 *   &g_sw dl(+0x10),&g_sw+0x1C
 * Info: proposed name GetLoChildDl (T1); TU lo
 * Notes: names_v2 v1 proposed SetLightFrustrumUp__FP5LIGHTP6VECTOR (0.003)
 * Confidence: mid (draft) [T1 0.65] - evidence: static naming pass; sources: r4_17F090
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
void func_001A5448(char *p, void **pa, void **pb)
{
    if (p) { *pa = p + 0x64; *pb = p + 0x70; }
    else { *pa = &D_003DD4A0; *pb = (char *)&D_003DD4A0 + 0xC; }
}
