struct AR { int cb; int cbAlign; };
struct E28 { char pad[0x28]; };
struct E20 { char pad[0x20]; };
struct Z { int a; short b; unsigned char c1; unsigned char c2; int pad8; struct E28 **ap1; struct E20 **ap2; };
extern struct E28 *D_003DCFF4;
extern struct E20 *D_003DCFFC;
void func_00117968(void *pbis, int cb, void *pv);
int func_00117C20(void *pbis);
void *func_001AE1D0(struct AR *par);
/** @sly2-annot addr=0x1EB1A8
 * Purpose: Read a TEX: header then bitmap indices (into D_003DCFF4) and clut indices (0xFFFF = none)
 * Info: proposed name LoadTexFromBrx (T1); TU shd
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r6b_rest+parse-verified
 * See: docs/engine/rendering.md#13-key-functions, docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
void func_001EB1A8(void *pbis, struct Z *pz)
{
    struct AR ar;
    int i;
    func_00117968(pbis, 8, pz);
    if (pz->c1) {
        ar.cb = pz->c1 * 4;
        ar.cbAlign = 0x10;
        pz->ap1 = func_001AE1D0(&ar);
        for (i = 0; i < pz->c1; i++)
            pz->ap1[i] = &D_003DCFF4[func_00117C20(pbis)];
    }
    if (pz->c2) {
        ar.cb = pz->c2 * 4;
        ar.cbAlign = 0x10;
        pz->ap2 = func_001AE1D0(&ar);
        for (i = 0; i < pz->c2; i++) {
            int k = func_00117C20(pbis);
            if (k == 0xFFFF) pz->ap2[i] = 0;
            else pz->ap2[i] = &D_003DCFFC[k];
        }
    }
}
