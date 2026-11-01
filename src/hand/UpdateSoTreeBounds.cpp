// UpdateSoTreeBounds (0x1F50E0): SO virtual 48 - rebuild the bounds of the SO's tree record (SO+0xEC).
// Pass 1, the box of every visible collision part: UpdateSoPartGeom (0x1F38A8) first, then a sphere part adds its
// XF position -/+ radius * D_002A2DE0, a mesh part its world points, and a part whose geometry is stale
// (nStampGeom != nStampWorld) also its local points from nUnk1c on, moved by the part's matrix.
// Pass 2, the sphere: centre = (min + max) * 0.5, radius = the farthest sphere edge / point from it.
// Then the world's +0x3CCC flag is set and 0x1F4BA8 is called.
// MATCH (T1583), no register pins. What it took (details in notes/1F0000.md):
// - ONE function-scope `int i` for all four point loops, each condition re-reading geom.cpos from memory;
// - the box loops go through ExtendBox(qword &, qword &, const qword &): the address-taken min/max make cse1's
//   cse_around_loop give up (stores to them), so the count copies come from gcse PRE as in the game, and loop.c
//   then leaves both flag masks in the part loop (ori/dsll32 at every test);
// - the radius code takes the centre as `const qword &` (gives the game's vmove copies of the centre), the sphere
//   position is read as `*(qword *)mat[3]` (aliases the VU_FLOAT home store, so it is read twice);
// - the transformed local points use GDistanceFused (vsub inside the dot-product asm, the 1.0 output first).
typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
struct MATRIX4 {float mat[4][4];};
struct VECTOR {float x,y,z,w;};
struct XF {char pad[0x40];MATRIX4 matWorld;};
struct GEOM {int cpos;VECTOR *apos;int csurf;void *asurf;int cedge;void *aedge;};
struct SOPART;
struct DLE {SOPART *next,*prev;};
struct SOPART
{
    /* 0x00 */ XF *pxf;
    /* 0x04 */ void *pso;
    /* 0x08 */ float gUnk08;          // sphere radius (fUnk49 parts)
    /* 0x0c */ int oid;
    /* 0x10 */ void *plvo;
    /* 0x14 */ short dhp;
    /* 0x16 */ ulong fUnk48:1;
               ulong fUnk49:1;        // the part is a sphere around its XF's position
               ulong fHidden:1;
               ulong unk_0x16:13;
    /* 0x18 */ unsigned grfzpk;
    /* 0x1c */ int nUnk1c;
    /* 0x20 */ MATRIX4 matWorld;
    /* 0x60 */ DLE dle;
    /* 0x68 */ char bspc[0x10];
    /* 0x78 */ GEOM geomLocal;
    /* 0x90 */ GEOM geomWorld;
    /* 0xa8 */ int nStampGeom;
    /* 0xac */ int nStampWorld;
};
struct SOTREE {qword qwMin,qwMax,qwCenter;float sRadius;SOPART *ppartFirst;};
struct SO {char pad[0xEC];SOTREE *ptree;};
struct SW {char pad[0x3CCC];int fUnk3CCC;};
struct VU_FLOAT {qword qw;};
extern qword D_002A2DE0,D_002A2DF0,D_002A2E00;
extern SW D_003DD490;
extern "C" void UpdateSoPartGeom(SOPART *);
extern "C" void func_001F4BA8(SO *);

static inline VU_FLOAT VuFloat(float g)
{
    VU_FLOAT v;
    __asm__("qmtc2 %1, %0" : "=j"(v.qw) : "r"(g));
    return v;
}
// qwBase + qwVec * g
static inline qword QwAddScaled(qword qwBase, qword qwVec, VU_FLOAT g)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qw) : "j"(qwBase), "j"(qwVec), "j"(g.qw));
    return qw;
}
static inline qword QwAddScaledF(qword qwBase, qword qwVec, float g)
{
    return QwAddScaled(qwBase, qwVec, VuFloat(g));
}
static inline qword QwTransformPos(MATRIX4 *pmat, qword qwPos)
{
    qword qwResult;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(qwResult)
            : "j"(*(qword *)pmat->mat[0]), "j"(*(qword *)pmat->mat[1]), "j"(*(qword *)pmat->mat[2]), "j"(*(qword *)pmat->mat[3]), "j"(qwPos));
    return qwResult;
}
static inline qword QwSub(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
static inline qword QwMin(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vmini.xyz %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
static inline qword QwMax(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vmax.xyz %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
// |v| (x, y, z)
static inline float GLength(qword v)
{
    qword qw;
    qword vfOne;
    float g;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(qw), "=j"(vfOne) : "j"(v));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    __asm__("qmfc2 %0, %1" : "=r"(g) : "j"(qw));
    return g;
}
static inline float GDistance(const qword &qwA, const qword &qwB)
{
    return GLength(QwSub(qwA, qwB));
}
// |a - b| with the subtraction inside the dot-product asm (the transformed local points)
static inline float GDistanceFused(qword qwA, qword qwB)
{
    qword qw;
    qword vfOne;
    float g;
    __asm__("vsub.xyzw %1, %2, %3\n\tvmul.xyz %1, %1, %1\n\tvaddw.x %0, $vf0, $vf0w\n\tvadday.x $ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z"
            : "=j"(vfOne), "=j"(qw) : "j"(qwA), "j"(qwB));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    __asm__("qmfc2 %0, %1" : "=r"(g) : "j"(qw));
    return g;
}
static inline void ExtendBox(qword &qwMin, qword &qwMax, const qword &qwPos)
{
    qwMin = QwMin(qwMin, qwPos);
    qwMax = QwMax(qwMax, qwPos);
}

extern "C" void UpdateSoTreeBounds(SO *pso)
{
    qword qwMin = D_002A2DF0;
    qword qwMax = D_002A2E00;
    SOPART *ppart;
    int i;
    for (ppart = pso->ptree->ppartFirst; ppart; ppart = ppart->dle.next)
    {
        if (ppart->fHidden)
            continue;
        UpdateSoPartGeom(ppart);
        if (ppart->fUnk49)
        {
            float gRadius = ppart->gUnk08;
            qword qwLo = QwAddScaledF(*(qword *)ppart->pxf->matWorld.mat[3], D_002A2DE0, -gRadius);
            qwMin = QwMin(qwMin, qwLo);
            qword qwHi = QwAddScaledF(*(qword *)ppart->pxf->matWorld.mat[3], D_002A2DE0, gRadius);
            qwMax = QwMax(qwMax, qwHi);
        }
        else
        {
            for (i = 0; i < ppart->geomWorld.cpos; i++)
            {
                qword qwPos = *(qword *)&ppart->geomWorld.apos[i];
                ExtendBox(qwMin, qwMax, qwPos);
            }
        }
        if (ppart->nStampGeom != ppart->nStampWorld)
        {
            for (i = ppart->nUnk1c; i < ppart->geomWorld.cpos; i++)
            {
                qword qwPos = QwTransformPos(&ppart->matWorld, *(qword *)&ppart->geomLocal.apos[i]);
                ExtendBox(qwMin, qwMax, qwPos);
            }
        }
    }

    pso->ptree->qwMin = qwMin;
    pso->ptree->qwMax = qwMax;
    VU_FLOAT gHalf = VuFloat(0.5f);
    float sRadius = 0.0f;
    qword qwCenter;
    __asm__("vmulax.xyzw $ACC, %1, %3x\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qwCenter) : "j"(qwMin), "j"(qwMax), "j"(gHalf.qw));
    for (ppart = pso->ptree->ppartFirst; ppart; ppart = ppart->dle.next)
    {
        if (ppart->fUnk49)
        {
            float gDist = GDistance(qwCenter, *(qword *)ppart->pxf->matWorld.mat[3]) + ppart->gUnk08;
            if (sRadius < gDist)
                sRadius = gDist;
        }
        else
        {
            for (i = 0; i < ppart->geomWorld.cpos; i++)
            {
                float gDist = GDistance(qwCenter, *(qword *)&ppart->geomWorld.apos[i]);
                if (sRadius < gDist)
                    sRadius = gDist;
            }
        }
        if (ppart->nStampGeom != ppart->nStampWorld)
        {
            for (i = ppart->nUnk1c; i < ppart->geomWorld.cpos; i++)
            {
                qword qwPos = QwTransformPos(&ppart->matWorld, *(qword *)&ppart->geomLocal.apos[i]);
                float gDist = GDistanceFused(qwCenter, qwPos);
                if (sRadius < gDist)
                    sRadius = gDist;
            }
        }
    }
    pso->ptree->qwCenter = qwCenter;
    pso->ptree->sRadius = sRadius;
    D_003DD490.fUnk3CCC = 1;
    func_001F4BA8(pso);
}

typedef char layout_part_size[(sizeof(SOPART)==0xB0)?1:-1];
typedef char layout_part_matrix[((unsigned)&((SOPART*)0)->matWorld==0x20)?1:-1];
typedef char layout_part_dle[((unsigned)&((SOPART*)0)->dle==0x60)?1:-1];
typedef char layout_part_world[((unsigned)&((SOPART*)0)->geomWorld==0x90)?1:-1];
typedef char layout_part_stamp[((unsigned)&((SOPART*)0)->nStampWorld==0xAC)?1:-1];
typedef char layout_tree_first[((unsigned)&((SOTREE*)0)->ppartFirst==0x34)?1:-1];
