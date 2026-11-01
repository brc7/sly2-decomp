struct MATRIX4 { float mat[4][4]; } __attribute__((aligned(16)));
struct BONE { char pad[0x40]; MATRIX4 mat; };
class LO {
public:
    int a, b, c;
#define V(n) virtual void V##n();
    V(1) V(2) V(3) V(4) V(5) V(6) V(7) V(8) V(9) V(10)
    V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19) V(20)
    V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30)
    V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39) V(40)
    V(41) V(42) V(43) V(44) V(45) V(46) V(47) V(48) V(49) V(50)
#undef V
    virtual BONE *PboneFind(int n);
};
struct ITEM { int oid; LO *plo; int n8; BONE *pbone; MATRIX4 mat; };
struct GRP { int a; BONE *pbone; int pad[4]; ITEM *aitem; int pad2[3]; };
struct HDR { signed char c0; signed char citem; char pad[0x12]; };
struct X { char pad[8]; int oid; char pad2[0x9C - 0xC]; X *pxNext; };
struct S { void *pvOwner; char pad[0x38 - 4]; int cgrp; HDR *ahdr; GRP *agrp; char pad2[0x4C - 0x44]; int f4C; char pad3[0x58 - 0x50]; X *pxFirst; };
struct G;
extern G D_003DD490;
extern int D_003E11EC;
extern "C" LO *func_0015E938(G *pg, int kind, int oid, void *owner);
extern "C" void InvertMatrix4Rigid(MATRIX4 *pmatDst, MATRIX4 *pmatSrc);
extern "C" void __ml__FRC7MATRIX4T0(MATRIX4 *pmatDst, MATRIX4 *pmatA, MATRIX4 *pmatB);
extern "C" void *func_002333B4(void *pv, int n, int cb);
extern "C" void func_0022B7A0(LO *plo, X *px);
extern "C" void func_00167B68(S *p)
{
    if (p->f4C != -1)
        p->f4C = D_003E11EC;
    for (int i = 0; i < p->cgrp; i++) {
        HDR *phdr = &p->ahdr[i];
        GRP *pgrp = &p->agrp[i];
        for (int j = 0; j < phdr->citem; j++) {
            ITEM *pitem = &pgrp->aitem[j];
            pitem->plo = func_0015E938(&D_003DD490, 4, pitem->oid, p->pvOwner);
            if (pitem->plo)
                pitem->pbone = pitem->plo->PboneFind(pitem->n8);
            if (pitem->pbone) {
                MATRIX4 mat;
                if (pgrp->pbone) {
                    MATRIX4 matT;
                    InvertMatrix4Rigid(&matT, &pitem->pbone->mat);
                    __ml__FRC7MATRIX4T0(&mat, &matT, &pgrp->pbone->mat);
                } else {
                    InvertMatrix4Rigid(&mat, &pitem->pbone->mat);
                }
                pitem->mat = mat;
            } else if (pgrp->pbone) {
                pitem->mat = pgrp->pbone->mat;
            } else {
                MATRIX4 *pmat = &pitem->mat;
                func_002333B4(pmat, 0, sizeof(MATRIX4));
                pmat->mat[0][0] = pmat->mat[1][1] = pmat->mat[2][2] = pmat->mat[3][3] = 1.0f;
            }
        }
    }
    for (X *px = p->pxFirst; px; px = px->pxNext) {
        LO *plo = func_0015E938(&D_003DD490, 4, px->oid, p->pvOwner);
        if (plo)
            func_0022B7A0(plo, px);
    }
}
