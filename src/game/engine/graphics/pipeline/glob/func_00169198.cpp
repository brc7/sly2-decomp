// func_00169198 (Claude agent, lanes I1-I9, H21): fresh rewrite in the style of Lilly's matched files.
// H21: the copy loop is a `for` counter loop (loop.c turns it into the target's 12/-4/bgez count) preceded by an
// empty do-while(0), whose loop notes act as the scheduling barrier the target shows after `pb += 4`.
// Builds the VIF packet for every submodel of a model: model matrix, optional bone matrices, optional skin matrices,
// then queues the packet with its sort distance.
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char uchar;
typedef int qword __attribute__((mode(TI)));
struct MATRIX4
{
    float mat[4][4];
} __attribute__((aligned(16)));
struct VECTOR4
{
    float x, y, z, w;
} __attribute__((aligned(16)));
struct XF
{
    char pad[0x40];
    MATRIX4 mat;
};
struct VTENT
{
    short delta, pad;
    void (*pfn)(void *);
};
struct OBJ
{
    char pad[0xC];
    VTENT *pvt;
};
struct SHD
{
    uint aw[12];
};
struct ALO
{
    char pad[0x104];
    int cpshd;
    void *apshd[1];
};
struct BONE
{
    int oid;
    float g;
    short n, pad;
    XF *pxf;
    MATRIX4 mat;
};
struct SKIN
{
    char pad[0xC];
    char *pbOwner;
    int cmat;
    int fFirst;
    int fSecond;
    int pad1C;
    uint ib;
};
struct SUBDESC
{
    signed char kind;
    signed char cbone;
    short channel;
    float dist;
    void *pv;
    uint ib;
    uint cw;
};
struct SUB
{
    int n;
    XF *pxf;
    MATRIX4 *pmatParent;
    uint grf;
    char *pb;
    uint w;
    BONE *abone;
    SKIN *pskin;
    int pad20;
    qword *ppos;
};
struct SPH
{
    float pos[3];
    float rad;
    char pad[8];
};
struct MODEL
{
    char pad[0x24];
    int cobj;
    OBJ **apobj;
    char pad2C[8];
    uint grfMode;
    int csub;
    SUBDESC *asubdesc;
    SUB *asub;
    SPH *asph;
    char pad48[8];
    float scale;
};
struct DMA
{
    char pad[0xC];
    uchar *pb;
};
struct RPL
{
    int kind;
    float dist;
    uchar *pb;
    char *pv;
};
struct G;
extern DMA D_002DEA28;
extern G D_003DD490;
extern MATRIX4 D_002A2D30;
extern "C" {
void *func_002333B4(void *pv, int b, uint cb);
void *func_0023320C(void *pvDst, const void *pvSrc, uint cb);
void func_001A4AD8(G *pg, int c, void **apv, SHD *pshd);
int func_001EDA20(OBJ *pobj);
int func_001BEE10(ALO *palo, int n);
void func_001AE5A0(void *pvDst, const void *pvSrc, uint cb);
void __ml__FRC7MATRIX4T0(MATRIX4 *pmatDst, const MATRIX4 *pmatA, const MATRIX4 *pmatB);
void InvertMatrix4Rigid(MATRIX4 *pmatDst, const MATRIX4 *pmatSrc);
qword func_001A62E8(ALO *palo);
RPL *func_001CD998(void);
}
static inline MATRIX4 MulMat(const MATRIX4 &matA, const MATRIX4 &matB) return mat
{
    __ml__FRC7MATRIX4T0(&mat, &matA, &matB);
}
static inline void CopyMat(MATRIX4 &matDst, const MATRIX4 &matSrc)
{
    func_0023320C(&matDst, &matSrc, sizeof(MATRIX4));
}
static inline qword QwSub(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
static inline qword QwXfm(const MATRIX4 &mat, qword qwPos)
{
    qword qw;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(qw)
            : "j"(*(qword *)mat.mat[0]), "j"(*(qword *)mat.mat[1]), "j"(*(qword *)mat.mat[2]), "j"(*(qword *)mat.mat[3]), "j"(qwPos));
    return qw;
}
static inline void GBitsSqLengthTo(int &gDst, qword qwV)
{
    qword qwSq, qwOne;
    __asm__("vmul.xyz %1, %2, %2\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(qwOne), "=j"(qwSq) : "j"(qwV));
    // the "m"/"r" alternative is allocation-only (never chosen): it keeps the destination a MEM, so the qmfc2 result is an output reload
    __asm__("qmfc2.ni %0, %1" : "=r,m"(gDst) : "j,!r"(qwSq));
}
static inline int GBitsSqLength(qword qwV)
{
    qword qwSq, qwOne;
    int g;
    __asm__("vmul.xyz %1, %2, %2\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(qwOne), "=j"(qwSq) : "j"(qwV));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(qwSq));
    return g;
}
static inline void CopyQws(uint *pwDst, const uint *pwSrc, int cw)
{
    // empty do-while(0) (as left by a statement macro): its loop notes keep later setup below the tag/pb stores
    do
    {
    } while (0);
    for (int k = 0; k < cw; k += 4)
    {
        uint w0 = pwSrc[0];
        uint w1 = pwSrc[1];
        uint w2 = pwSrc[2];
        uint w3 = pwSrc[3];
        pwSrc += 4;
        pwDst[0] = w0;
        pwDst[1] = w1;
        pwDst[2] = w2;
        pwDst[3] = w3;
        pwDst += 4;
    }
}
template <uint TAG> static inline void UnpackQws(const void *pv, int cw)
{
    DMA *pdma = &D_002DEA28;
    *(uint *)pdma->pb = TAG;
    pdma->pb += 4;
    CopyQws((uint *)pdma->pb, (const uint *)pv, cw);
    pdma->pb += cw * 4;
}
static inline void UnpackQws(uint tag, const void *pv, int cw)
{
    DMA *pdma = &D_002DEA28;
    *(uint *)pdma->pb = tag;
    pdma->pb += 4;
    CopyQws((uint *)pdma->pb, (const uint *)pv, cw);
    pdma->pb += cw * 4;
}
/** @sly2-annot addr=0x169198
 * Purpose: Builds the VIF packet for every submodel of a model (model matrix, optional bone and skin
 *   matrices) and queues it with its sort distance
 * Info: proposed name DrawGlobsetSkinned (T2); TU glob
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/rendering.md#2-render-list-entries-rpl,
 *   docs/reference/functions/rendering.md#glob-0x167310-0x16f148,
 *   docs/reference/functions/rendering.md#tu_1be398-0x1be398-0x1bf110
 * @end-sly2-annot */
extern "C" void func_00169198(MODEL *pmodel, ALO *palo)
{
    SHD shd;
    MATRIX4 matModel;
    SHD *pshd;
    if (palo->cpshd)
    {
        func_002333B4(&shd, 0, sizeof(SHD));
        pshd = &shd;
        func_001A4AD8(&D_003DD490, palo->cpshd, palo->apshd, &shd);
    }
    else
    {
        pshd = 0;
    }
    int i;
    for (i = 0; i < pmodel->cobj; i++)
    {
        OBJ *pobj = pmodel->apobj[i];
        if (func_001EDA20(pobj))
        {
            VTENT *pvte = &pobj->pvt[8];
            pvte->pfn((char *)pobj + pvte->delta);
        }
    }
    for (i = 0; i < pmodel->csub; i++)
    {
        SUBDESC *pdesc = &pmodel->asubdesc[i];
        SUB *psub = &pmodel->asub[i];
        uint grf = psub->grf;
        if (grf & 0x800)
            continue;
        if ((grf & 7) == 2)
            continue;
        int kind = func_001BEE10(palo, (grf >> 3) & 0x1F);
        if (kind == -1)
            continue;
        if (psub->grf & 0x1000)
        {
            func_001AE5A0(&matModel, psub->pb + 0x14, sizeof(MATRIX4));
        }
        else
        {
            MATRIX4 *pmatParent = psub->pmatParent;
            MATRIX4 mat;
            CopyMat(mat, psub->pxf->mat);
            if (pmatParent)
                CopyMat(mat, MulMat(mat, *pmatParent));
            matModel = mat;
        }
        qword dpos;
        if (psub->ppos)
        {
            qword pos = *psub->ppos;
            dpos = QwSub(pos, func_001A62E8(palo));
        }
        else
        {
            VECTOR4 pos;
            func_001AE5A0(&pos, &pmodel->asph[i], 12);
            dpos = QwXfm(matModel, *(qword *)&pos);
        }
        uchar *pbStart = D_002DEA28.pb;
        if (psub->grf & 0x100)
        {
            *(ulong *)pbStart = (ulong)(int)(pshd->aw[5] | 0x40000000) | ((ulong)(int)pshd->aw[6] << 32);
            D_002DEA28.pb += 0x10;
        }
        ulong *ptag = (ulong *)D_002DEA28.pb;
        *ptag = 0x10000000;
        D_002DEA28.pb += 0x10;
        if (psub->abone)
            UnpackQws<0x6C04C000>((uint *)&D_002A2D30, 16);
        else
            UnpackQws<0x6C04C000>((uint *)&matModel, 16);
        VECTOR4 vecParams;
        vecParams.x = 1.0f;
        vecParams.y = 1.0f;
        vecParams.z = pmodel->scale;
        *(uint *)&vecParams.w = (psub->grf & 0x200) ? pmodel->grfMode : 0;
        UnpackQws<0x6C01C008>((uint *)&vecParams, 4);
        if (psub->abone)
        {
            for (int ibone = 0; ibone < pdesc->cbone; ibone++)
            {
                BONE *pbone = &psub->abone[ibone];
                MATRIX4 mat;
                if (pbone->pxf)
                    mat = MulMat(pbone->pxf->mat, pbone->mat);
                else
                    mat = pbone->mat;
                UnpackQws((ibone * 4 + pdesc->ib) | 0x6C04C000, (uint *)&mat, 16);
            }
        }
        SKIN *pskin = psub->pskin;
        if (pskin)
        {
            MATRIX4 matInv;
            InvertMatrix4Rigid(&matInv, &matModel);
            uint ib = pskin->ib;
            for (int imat = 0; imat < pskin->cmat; imat++)
            {
                if (pskin->fFirst)
                {
                    MATRIX4 mat;
                    MATRIX4 *pmatInv = &matInv;
                    int ibMat = imat * 0x40 + 0x6F0;
                    mat = MulMat(*pmatInv, *(MATRIX4 *)(pskin->pbOwner + ibMat));
                    UnpackQws(ib | 0x6C04C000, (uint *)&mat, 16);
                    ib += 4;
                }
                if (pskin->fSecond)
                {
                    UnpackQws(ib | 0x6C04C000, (uint *)(pskin->pbOwner + 0x7F0 + imat * 0x40), 16);
                    ib += 4;
                }
            }
        }
        {
            DMA *pdma = &D_002DEA28;
            for (uchar *pbPad = pdma->pb; (uint)pbPad & 0xF; pbPad = pdma->pb)
            {
                *(uint *)pbPad = 0;
                pdma->pb += 4;
            }
        }
        *(uint *)ptag |= ((int)(D_002DEA28.pb - (uchar *)ptag) >> 4) - 1;
        *(ulong *)D_002DEA28.pb = 0x60000000;
        D_002DEA28.pb += 0x10;
        RPL *prpl = func_001CD998();
        prpl->kind = kind;
        if (psub->grf & 0x400)
            prpl->dist = pdesc->dist;
        else
            GBitsSqLengthTo(*(int *)&prpl->dist, dpos);
        prpl->pb = pbStart;
        prpl->pv = psub->pb;
    }
}
