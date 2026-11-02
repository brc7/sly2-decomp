struct AR { int cb; int cbAlign; };
struct E { int id; unsigned char b4; char pad[0x20 - 5]; };
extern int D_003DCFF8;
extern struct E *D_003DCFFC;
extern int D_003DCF9C;
int func_00117C20(void *pbis);
void *func_001AE378(struct AR *par);
void func_001EA8A8(void *pbis, struct E *pe);
/** @sly2-annot addr=0x1EAE40
 * Purpose: Read count, allocate 0x20-byte CLUT array (D_003DCFFC), LoadClutFromBrx each, track shared count
 * Info: proposed name LoadColorTablesFromBrx (T1); TU shd
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels |
 *   textures format doc cites this function by this name (CONFIRMED(code) in the doc)
 * Confidence: high [T1 0.85] - evidence: static analysis; sources:
 *   r6b_rest+parse-verified+format-doc:textures
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/formats/level-objects.md#2-per-world-entry-header-loop-body-of-loadswfrombrx,
 *   docs/formats/textures.md#1-layout-inside-a-world-entry-cstatic--cparse
 * @end-sly2-annot */
void func_001EAE40(void *pbis)
{
    struct AR ar;
    int i = 0;
    D_003DCFF8 = func_00117C20(pbis);
    ar.cb = D_003DCFF8 * sizeof(struct E);
    ar.cbAlign = 0x10;
    D_003DCFFC = func_001AE378(&ar);
    D_003DCF9C = 0;
    for (; i < D_003DCFF8; i++) {
        func_001EA8A8(pbis, &D_003DCFFC[i]);
        if (D_003DCFFC[i].b4 == 1) D_003DCF9C++;
    }
    if (D_003DCF9C < D_003DCFF8 - D_003DCF9C) D_003DCF9C = D_003DCFF8 - D_003DCF9C;
}
