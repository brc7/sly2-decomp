struct ZPR { char ab[0x630]; };
struct F { char ab[0x140]; };
extern struct F D_002DE770[];
extern int D_002BE320[];
void func_001703F8(struct ZPR *pzpr, int a, int b);
void func_0015F710(struct F *pf, int n, struct ZPR *pzpr);
/** @sly2-annot addr=0x15EF30
 * Purpose: InitGsb(gsb,0x1000,0x1800) then sets up the two built-in CFontBrx fonts into that GS memory
 *   (0x15F710)
 * Info: proposed name PostFontsLoad (T1); TU font
 * Notes: names_v2 v1 proposed FUN_0015c200 (0.000)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r3_1539A8
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#font-0x15ef00-0x1603b8
 * @end-sly2-annot */
void func_0015EF30(void)
{
    struct ZPR zpr;
    int i;
    func_001703F8(&zpr, 0x1000, 0x1800);
    for (i = 0; i < 2; i++) {
        int n = D_002BE320[i];
        func_0015F710(&D_002DE770[i], n, &zpr);
    }
}
