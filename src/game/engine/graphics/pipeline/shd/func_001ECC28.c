struct BS { int n0; int n4; int n8; };
struct X { char pad[0xC]; int c; };
struct T1 { char pad[0x18]; long l18; };
struct T2 { char pad[0x20]; long l20; };
struct E1 { int n0; unsigned char b4; char pad5[5]; unsigned short hA; int nC; int n10; char pad14[4]; long l18; };
struct E2 { int n0; unsigned char b4; char pad5[7]; unsigned char bC; char padD; unsigned short hE; char pad10[4]; int n14; int n18; char pad1C[4]; long l20; };
extern int D_003DCFF8;
extern struct E1 *D_003DCFFC;
extern int D_003DCFF0;
extern struct E2 *D_003DCFF4;
struct T1 *func_001ED5D0(int);
struct T2 *func_001ED4E0(int);
int IgsAllocGsb__FP3GSBi(struct BS *, int);
void func_001EC9D0(struct E1 *, int);
void func_001ECAB0(struct E1 *, int, struct X *);
void func_001ECB18(struct E2 *, int);
void func_001ECBF0(struct E2 *, int, struct X *);
void func_00157500(struct X *, int, int);
void func_001ED560(struct E1 *);
void func_001ED470(struct E2 *);
/** @sly2-annot addr=0x1ECC28
 * Purpose: Upload CLUTs/BMPs of kind k into a GS block; returns whether anything was added
 * Info: proposed name FBuildUploadBitmapGifs (T1); TU shd
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r6b_rest+verdicts_B
 * See: docs/engine/rendering.md#9-textures-cluts-and-gs-memory-shdcpp-0x1ea568-gscpp-0x16f2d0-cstatic,
 *   docs/engine/rendering.md#13-key-functions,
 *   docs/guides/port-plan.md#63-textures-k8-renderingmd-9-formatstexturesmd
 * @end-sly2-annot */
int func_001ECC28(int k, struct BS *pbs, struct X *px)
{
    int c0 = 0;
    int i, j;
    if (px)
        c0 = px->c;
    for (i = 0; i < D_003DCFF8; i++) {
        struct E1 *pe = &D_003DCFFC[i];
        struct T1 *pt;
        int v;
        if (k != pe->b4)
            continue;
        pt = func_001ED5D0(pe->n0);
        if (pt) {
            pe->l18 = pt->l18;
            continue;
        }
        v = IgsAllocGsb__FP3GSBi(pbs, pe->hA);
        func_001EC9D0(pe, v);
        if (px && pbs->n4 <= pbs->n8) {
            func_001ECAB0(pe, v, px);
            func_00157500(px, pe->n10, pe->nC);
        }
        func_001ED560(pe);
    }
    for (j = 0; j < D_003DCFF0; j++) {
        struct E2 *pe = &D_003DCFF4[j];
        struct T2 *pt;
        int v;
        if (k != pe->b4)
            continue;
        pt = func_001ED4E0(pe->n0);
        if (pt) {
            pe->l20 = pt->l20;
            continue;
        }
        if (!pe->bC) {
            int r = pbs->n4 & 0x1F;
            if (r)
                IgsAllocGsb__FP3GSBi(pbs, 32 - r);
        }
        v = IgsAllocGsb__FP3GSBi(pbs, pe->hE);
        func_001ECB18(pe, v);
        if (px && pbs->n4 <= pbs->n8) {
            func_001ECBF0(pe, v, px);
            func_00157500(px, pe->n18, pe->n14);
        }
        func_001ED470(pe);
    }
    if (!px)
        return 0;
    return px->c != c0;
}
