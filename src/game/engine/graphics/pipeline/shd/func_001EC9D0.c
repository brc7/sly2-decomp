typedef unsigned long ulong;
struct TEX0 {
    ulong TBP0 : 14; ulong TBW : 6; ulong PSM : 6; ulong TW : 4; ulong TH : 4; ulong TCC : 1;
    ulong TFX : 2; ulong CBP : 14; ulong CPSM : 4; ulong CSM : 1; ulong CSA : 5; ulong CLD : 3;
};
struct CLUT { int pad[2]; unsigned short c; char pad2[0x18 - 0xA]; struct TEX0 tex0; };
/** @sly2-annot addr=0x1EC9D0
 * Purpose: Fill TEX2-style CLUT fields
 * Info: proposed name BuildClutTex2 (T1); TU shd
 * Notes: textures format doc cites this function by this name (CONFIRMED(code) in the doc)
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r6b_rest+verdicts_A2+format-doc:textures
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/formats/textures.md#2-clut-record-0x10-byte-header-cstatic,
 *   docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
void func_001EC9D0(struct CLUT *pclut, int cbp)
{
    pclut->tex0.CBP = cbp;
    pclut->tex0.PSM = (pclut->c <= 0x10) ? 0x14 : 0x13;
    pclut->tex0.CPSM = 0;
    pclut->tex0.CSM = 0;
    pclut->tex0.CSA = 0;
    pclut->tex0.CLD = 1;
}
