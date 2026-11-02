struct B { int a[12]; };
struct MK { int a; };
extern int D_003DCF9C, D_003DCF98;
extern int D_002DE598;
void __4GIFS(struct B *pb);
struct MK *STACKSCOPE_ctor(struct MK *pm);
void STACKSCOPE_ctor_1AE4A0(struct MK *pm, int n);
void AllocSw__4DMASi(struct B *pb, int n);
void AddDmaCnt__4DMAS(struct B *pb);
int FBuildUploadBitmapGifs__FiP3GSBP4GIFS(int a, void *p, struct B *pb);
void AddPrimPack__4GIFSiiUl(struct B *pb, int a, int b, int c);
void PackAD__4GIFSll(struct B *pb, int a, int b);
void AddPrimEnd__4GIFS(struct B *pb);
void AddDmaEnd__4DMAS(struct B *pb);
void Detach__4DMASPiPP2QW(struct B *pb, int a, int *pn);
void SendDmaSyncGsFinish__FP10sceDmaChanP2QW(int a, int b);
/** @sly2-annot addr=0x170730
 * Purpose: Builds and sends (sync) a GIF chain uploading every level bitmap/CLUT of class 1 into GS memory
 *   allocated from the GSB, then TEXFLUSH + FINISH
 * Info: proposed name UploadBitmaps (T1); TU gs
 * Notes: textures format doc cites this function by this name (CONFIRMED(code) in the doc)
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r3_1539A8+format-doc:textures
 * See: docs/engine/rendering.md#9-textures-cluts-and-gs-memory-shdcpp-0x1ea568-gscpp-0x16f2d0-cstatic,
 *   docs/engine/world-sw.md#10-key-functions,
 *   docs/formats/textures.md#1-layout-inside-a-world-entry-cstatic--cparse
 * @end-sly2-annot */
void UploadBitmaps__FP3GSB(void *p) {
    struct B b;
    struct MK mk;
    int n;
    __4GIFS(&b);
    STACKSCOPE_ctor(&mk);
    AllocSw__4DMASi(&b, ((D_003DCF9C * 3 + D_003DCF98) << 3) | 4);
    AddDmaCnt__4DMAS(&b);
    if (FBuildUploadBitmapGifs__FiP3GSBP4GIFS(1, p, &b)) {
        AddPrimPack__4GIFSiiUl(&b, 0, 1, 0xE);
        PackAD__4GIFSll(&b, 0x3F, 0);
        PackAD__4GIFSll(&b, 0x61, 0);
        AddPrimEnd__4GIFS(&b);
        AddDmaEnd__4DMAS(&b);
        Detach__4DMASPiPP2QW(&b, 0, &n);
        SendDmaSyncGsFinish__FP10sceDmaChanP2QW(D_002DE598, n);
    }
    STACKSCOPE_ctor_1AE4A0(&mk, 2);
}
