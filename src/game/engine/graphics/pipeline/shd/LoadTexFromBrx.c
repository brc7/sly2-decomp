struct AR { int cb; int cbAlign; };
struct E28 { char pad[0x28]; };
struct E20 { char pad[0x20]; };
struct Z { int a; short b; unsigned char c1; unsigned char c2; int pad8; struct E28 **ap1; struct E20 **ap2; };
extern struct E28 *D_003DCFF4;
extern struct E20 *D_003DCFFC;
void Read__18CBinaryInputStreamiPv(void *pbis, int cb, void *pv);
int U16Read__18CBinaryInputStream(void *pbis);
void *func_001AE1D0(struct AR *par);
/** @sly2-annot addr=0x1EB1A8
 * Purpose: Read a TEX: header then bitmap indices (into D_003DCFF4) and clut indices (0xFFFF = none)
 * Info: proposed name LoadTexFromBrx (T1); TU shd
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r6b_rest+parse-verified
 * See: docs/engine/rendering.md#13-key-functions, docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
void LoadTexFromBrx(void *pbis, struct Z *pz)
{
    struct AR ar;
    int i;
    Read__18CBinaryInputStreamiPv(pbis, 8, pz);
    if (pz->c1) {
        ar.cb = pz->c1 * 4;
        ar.cbAlign = 0x10;
        pz->ap1 = func_001AE1D0(&ar);
        for (i = 0; i < pz->c1; i++)
            pz->ap1[i] = &D_003DCFF4[U16Read__18CBinaryInputStream(pbis)];
    }
    if (pz->c2) {
        ar.cb = pz->c2 * 4;
        ar.cbAlign = 0x10;
        pz->ap2 = func_001AE1D0(&ar);
        for (i = 0; i < pz->c2; i++) {
            int k = U16Read__18CBinaryInputStream(pbis);
            if (k == 0xFFFF) pz->ap2[i] = 0;
            else pz->ap2[i] = &D_003DCFFC[k];
        }
    }
}
