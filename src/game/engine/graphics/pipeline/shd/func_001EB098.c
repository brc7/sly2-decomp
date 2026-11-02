struct AR { int cb; int cbAlign; };
struct E { int id; unsigned char b4; char pad[0x28 - 5]; };
extern int D_003DCFF0;
extern struct E *D_003DCFF4;
extern int D_003DCF98;
int func_00117C20(void *pbis);
void *func_001AE378(struct AR *par);
void func_001EAF50(void *pbis, struct E *pe);
/** @sly2-annot addr=0x1EB098
 * Purpose: Read count, allocate 0x28-byte BMP array (D_003DCFF4), LoadBmpFromBrx each
 * Info: proposed name LoadBitmapsFromBrx (T1); TU shd
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels |
 *   textures format doc cites this function by this name (CONFIRMED(code) in the doc)
 * Confidence: high [T1 0.85] - evidence: static analysis; sources:
 *   r6b_rest+parse-verified+format-doc:textures
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/formats/level-objects.md#2-per-world-entry-header-loop-body-of-loadswfrombrx,
 *   docs/formats/textures.md#1-layout-inside-a-world-entry-cstatic--cparse
 * @end-sly2-annot */
void func_001EB098(void *pbis)
{
    struct AR ar;
    int i = 0;
    D_003DCFF0 = func_00117C20(pbis);
    ar.cb = D_003DCFF0 * sizeof(struct E);
    ar.cbAlign = 0x10;
    D_003DCFF4 = func_001AE378(&ar);
    D_003DCF98 = 0;
    for (; i < D_003DCFF0; i++) {
        func_001EAF50(pbis, &D_003DCFF4[i]);
        if (D_003DCFF4[i].b4 == 1) D_003DCF98++;
    }
    if (D_003DCF98 < D_003DCFF0 - D_003DCF98) D_003DCF98 = D_003DCFF0 - D_003DCF98;
}
