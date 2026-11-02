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
void func_00166920(struct DMAS *);
void func_00226C10(struct X *);
struct M *STACKSCOPE_ctor(struct M *);
void STACKSCOPE_ctor_1AE4A0(struct M *, int);
void func_001572E0(struct X *, int);
void func_001574B8(struct X *);
void func_00226D10(struct X *);
void func_001572D0(struct DMAS *);
void func_00157330(struct DMAS *, int, void *);
int IgsAllocGsb__FP3GSBi(struct BS *, int);
void func_001EC9D0(struct E1 *, int);
void func_001ECAB0(struct E1 *, int, struct DMAS *);
void func_001ECB18(struct E2 *, int);
void func_001ECBF0(struct E2 *, int, struct DMAS *);
void func_00157348(struct DMAS *, int *, int *);
void func_002270C8(struct X *, int, int, int);
void func_00227148(struct X *, int, int, int);
void func_00166BD8(struct DMAS *, int, int, int);
void func_00166E90(struct DMAS *, int, int);
void func_00166968(struct DMAS *);
void func_001576F0(struct X *);
void func_00157620(struct X *);
void func_001573C0(struct X *, int, int, int *);
/** @sly2-annot addr=0x1ECE58
 * Purpose: Build and send DMA uploading the permanent (always-resident) bitmaps/CLUTs
 * Info: proposed name UploadPermShaders (T1); TU shd
 * Notes: textures format doc cites this function by this name (CONFIRMED(code) in the doc)
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r6b_rest+format-doc:textures
 * See: docs/engine/rendering.md#9-textures-cluts-and-gs-memory-shdcpp-0x1ea568-gscpp-0x16f2d0-cstatic,
 *   docs/engine/rendering.md#13-key-functions,
 *   docs/guides/port-plan.md#63-textures-k8-renderingmd-9-formatstexturesmd
 * @end-sly2-annot */
int func_001ECE58(struct BS *pbs, int k)
{
    struct DMAS dmas;
    struct X x;
    struct M m;
    char ab[0xA0];
    int cb, pv;
    int result;
    int n4;
    int i, j;
    func_00166920(&dmas);
    result = 0;
    func_00226C10(&x);
    n4 = pbs->n4;
    STACKSCOPE_ctor(&m);
    func_001572E0(&x, (D_003DCF9C * 3 + D_003DCF98) * 10 + 4);
    func_001574B8(&x);
    func_00226D10(&x);
    for (i = 0; i < D_003DCFF8; i++) {
        struct E1 *pe = &D_003DCFFC[i];
        int v;
        if (pe->b4 != k)
            continue;
        func_001572D0(&dmas);
        func_00157330(&dmas, 10, ab);
        v = IgsAllocGsb__FP3GSBi(pbs, pe->hA);
        func_001EC9D0(pe, v);
        func_001ECAB0(pe, v, &dmas);
        *(ulong *)(dmas.pqw - 1) |= 0x8000;
        func_00157348(&dmas, &cb, &pv);
        func_002270C8(&x, cb, pv, 0);
        func_00227148(&x, pe->n10, pe->nC, 0);
    }
    for (j = 0; j < D_003DCFF0; j++) {
        struct E2 *pe = &D_003DCFF4[j];
        int v;
        if (k != pe->b4)
            continue;
        func_001572D0(&dmas);
        func_00157330(&dmas, 10, ab);
        v = IgsAllocGsb__FP3GSBi(pbs, pe->hE);
        func_001ECB18(pe, v);
        func_001ECBF0(pe, v, &dmas);
        *(ulong *)(dmas.pqw - 1) |= 0x8000;
        func_00157348(&dmas, &cb, &pv);
        func_002270C8(&x, cb, pv, 0);
        func_00227148(&x, pe->n18, pe->n14, 0);
    }
    if (n4 < pbs->n4) {
        func_001572D0(&dmas);
        func_00157330(&dmas, 10, ab);
        func_00166BD8(&dmas, 0, 1, 14);
        func_00166E90(&dmas, 0x3F, 0);
        func_00166968(&dmas);
        func_00157348(&dmas, &cb, &pv);
        func_002270C8(&x, cb, pv, 0);
        func_00226D10(&x);
        func_001576F0(&x);
        func_00157620(&x);
        func_001573C0(&x, 0, 0, &result);
    }
    {
        int r = result;
        STACKSCOPE_ctor_1AE4A0(&m, 2);
        return r;
    }
}
