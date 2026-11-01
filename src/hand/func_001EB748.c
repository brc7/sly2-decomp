struct MSG { int cb; int nKind; };
struct E { char pad[5]; unsigned char b5; char pad2[0x2C - 6]; };
extern int D_003DD000;
extern void **D_003DD004;
extern int D_003DCFE8;
extern struct E *D_003DCFEC;
extern int D_003DCFA0;
extern void *D_003DCFA8[16];
extern char D_003E1590[];
void func_001EAE40(void *);
void func_001EB098(void *);
int func_00117C20(void *);
void *func_001AE378(struct MSG *);
void *func_001AE2A0(struct MSG *);
void *func_001ED7A8(int, void *);
void func_001EB2F0(void *, struct E *);
void LoadSwSharedDataFromBrx(void *, void *);
void *func_001ECE58(int, int);
void func_001EB960(void);
void func_001EB678(struct E *, void *);
void func_0011A070(void *, void *);
/** @sly2-annot addr=0x1EB748
 * Purpose: Load color tables, bitmaps, shaders and shader animations (PsaaLoadFromBrx 0x1ED7A8); upload
 *   permanent textures (0x1ECE58); build shader packets
 * Info: proposed name LoadShadersFromBrx (T1); TU shd
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels |
 *   textures format doc cites this function by this name (CONFIRMED(code) in the doc)
 * Confidence: high [T1 0.80] - evidence: static analysis; sources:
 *   r6b_rest+parse-verified+format-doc:textures
 * See: docs/engine/rendering.md#9-textures-cluts-and-gs-memory-shdcpp-0x1ea568-gscpp-0x16f2d0-cstatic,
 *   docs/engine/rendering.md#13-key-functions,
 *   docs/engine/world-sw.md#loadswfrombrx-0x13da18-matched-hand-file
 * @end-sly2-annot */
void func_001EB748(void *p, int k, void *pbis)
{
    struct MSG msg;
    int i, j;
    void *pv;
    func_001EAE40(pbis);
    func_001EB098(pbis);
    D_003DD000 = func_00117C20(pbis);
    msg.cb = D_003DD000 * 4;
    msg.nKind = 0x10;
    D_003DD004 = func_001AE378(&msg);
    for (i = 0; i < D_003DD000; i++)
        D_003DD004[i] = func_001ED7A8(0, pbis);
    D_003DCFE8 = func_00117C20(pbis);
    msg.cb = D_003DCFE8 * 0x2C;
    msg.nKind = 0x10;
    D_003DCFEC = func_001AE378(&msg);
    for (j = 0; j < D_003DCFE8; j++)
        func_001EB2F0(pbis, &D_003DCFEC[j]);
    LoadSwSharedDataFromBrx(p, pbis);
    pv = func_001ECE58(k, 3);
    if (pv && (unsigned)D_003DCFA0 < 16)
        D_003DCFA8[D_003DCFA0++] = pv;
    func_001EB960();
    for (i = 0; i < D_003DCFE8; i++) {
        struct E *pe = &D_003DCFEC[i];
        if (pe->b5 & 0x40) {
            void *q;
            msg.cb = 0x1C;
            msg.nKind = 0x10;
            q = func_001AE2A0(&msg);
            func_001EB678(pe, q);
            func_0011A070(D_003E1590, q);
        }
    }
}
