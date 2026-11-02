typedef unsigned long ulong;
typedef int qword __attribute__((mode(TI)));
struct BS { int n0; int n4; int n8; };
struct DMAS { int pad[3]; qword *pqw; int pad2[8]; };
struct X { int pad[12]; };
struct M { int pad[4]; };
struct E1 { int n0; unsigned char b4; char pad5[5]; unsigned short hA; int nC; int n10; char pad14[0xC]; };
struct E2 { int n0; unsigned char b4; char pad5[9]; unsigned short hE; char pad10[4]; int n14; int n18; char pad1C[0xC]; };
extern int D_003DCF98;
extern int D_003DCF9C;
extern int D_003DCFF8;
extern struct E1 *D_003DCFFC;
extern int D_003DCFF0;
extern struct E2 *D_003DCFF4;
void __4GIFS(struct DMAS *);
void __4VIFS(struct X *);
struct M *STACKSCOPE_ctor(struct M *);
void STACKSCOPE_ctor_1AE4A0(struct M *, int);
void AllocStack__4DMASi(struct X *, int);
void AddDmaCnt__4DMAS(struct X *);
void AddVifFlush__4VIFS(struct X *);
void Reset__4DMAS(struct DMAS *);
void AllocStatic__4DMASiP2QW(struct DMAS *, int, void *);
int IgsAllocGsb__FP3GSBi(struct BS *, int);
void BuildClutTex2__FP4CLUTi(struct E1 *, int);
void BuildClutGifs__FP4CLUTiP4GIFS(struct E1 *, int, struct DMAS *);
void BuildBmpTex0__FP3BMPi(struct E2 *, int);
void BuildBmpGifs(struct E2 *, int, struct DMAS *);
void Detach__4DMASPiPP2QW(struct DMAS *, int *, int *);
void AddVifDirect__4VIFSiP2QWi(struct X *, int, int, int);
void AddVifDirectRefs__4VIFSiP2QWi(struct X *, int, int, int);
void AddPrimPack__4GIFSiiUl(struct DMAS *, int, int, int);
void PackAD__4GIFSll(struct DMAS *, int, int);
void AddPrimEnd__4GIFS(struct DMAS *);
void EndDmaCnt__4DMAS(struct X *);
void AddDmaRet__4DMAS(struct X *);
void DetachCopySw__4DMASPiPP2QWT2(struct X *, int, int, int *);
/** @sly2-annot addr=0x1ECE58
 * Purpose: Build and send DMA uploading the permanent (always-resident) bitmaps/CLUTs
 * Info: proposed name UploadPermShaders (T1); TU shd
 * Notes: textures format doc cites this function by this name (CONFIRMED(code) in the doc)
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r6b_rest+format-doc:textures
 * See: docs/engine/rendering.md#9-textures-cluts-and-gs-memory-shdcpp-0x1ea568-gscpp-0x16f2d0-cstatic,
 *   docs/engine/rendering.md#13-key-functions,
 *   docs/guides/port-plan.md#63-textures-k8-renderingmd-9-formatstexturesmd
 * @end-sly2-annot */
int UploadPermShaders(struct BS *pbs, int k)
{
    struct DMAS dmas;
    struct X x;
    struct M m;
    char ab[0xA0];
    int cb, pv;
    int result;
    int n4;
    int i, j;
    __4GIFS(&dmas);
    result = 0;
    __4VIFS(&x);
    n4 = pbs->n4;
    STACKSCOPE_ctor(&m);
    AllocStack__4DMASi(&x, (D_003DCF9C * 3 + D_003DCF98) * 10 + 4);
    AddDmaCnt__4DMAS(&x);
    AddVifFlush__4VIFS(&x);
    for (i = 0; i < D_003DCFF8; i++) {
        struct E1 *pe = &D_003DCFFC[i];
        int v;
        if (pe->b4 != k)
            continue;
        Reset__4DMAS(&dmas);
        AllocStatic__4DMASiP2QW(&dmas, 10, ab);
        v = IgsAllocGsb__FP3GSBi(pbs, pe->hA);
        BuildClutTex2__FP4CLUTi(pe, v);
        BuildClutGifs__FP4CLUTiP4GIFS(pe, v, &dmas);
        *(ulong *)(dmas.pqw - 1) |= 0x8000;
        Detach__4DMASPiPP2QW(&dmas, &cb, &pv);
        AddVifDirect__4VIFSiP2QWi(&x, cb, pv, 0);
        AddVifDirectRefs__4VIFSiP2QWi(&x, pe->n10, pe->nC, 0);
    }
    for (j = 0; j < D_003DCFF0; j++) {
        struct E2 *pe = &D_003DCFF4[j];
        int v;
        if (k != pe->b4)
            continue;
        Reset__4DMAS(&dmas);
        AllocStatic__4DMASiP2QW(&dmas, 10, ab);
        v = IgsAllocGsb__FP3GSBi(pbs, pe->hE);
        BuildBmpTex0__FP3BMPi(pe, v);
        BuildBmpGifs(pe, v, &dmas);
        *(ulong *)(dmas.pqw - 1) |= 0x8000;
        Detach__4DMASPiPP2QW(&dmas, &cb, &pv);
        AddVifDirect__4VIFSiP2QWi(&x, cb, pv, 0);
        AddVifDirectRefs__4VIFSiP2QWi(&x, pe->n18, pe->n14, 0);
    }
    if (n4 < pbs->n4) {
        Reset__4DMAS(&dmas);
        AllocStatic__4DMASiP2QW(&dmas, 10, ab);
        AddPrimPack__4GIFSiiUl(&dmas, 0, 1, 14);
        PackAD__4GIFSll(&dmas, 0x3F, 0);
        AddPrimEnd__4GIFS(&dmas);
        Detach__4DMASPiPP2QW(&dmas, &cb, &pv);
        AddVifDirect__4VIFSiP2QWi(&x, cb, pv, 0);
        AddVifFlush__4VIFS(&x);
        EndDmaCnt__4DMAS(&x);
        AddDmaRet__4DMAS(&x);
        DetachCopySw__4DMASPiPP2QWT2(&x, 0, 0, &result);
    }
    {
        int r = result;
        STACKSCOPE_ctor_1AE4A0(&m, 2);
        return r;
    }
}
