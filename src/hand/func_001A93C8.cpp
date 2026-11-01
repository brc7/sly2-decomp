// func_001A93C8: ported from Lilly's personal Sly 2 decomp (src/P2/cplane.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// ---- CplaneFromSphere 0x1A93C8 (MATCH, 472 instructions; T1304 draft 48%, T1341 99.4%, T1396 the last mtc1) ----
// The contacts of a sphere (centre pos, radius sRadius) with the camera hull of a collision part: the first pbspLimit nodes of ppart->bspc.absp. At most
// ccontactMax CMCONTACTs (bsp.h) go to acontact; returns how many. Callers: SquishCmEye 0x14EDA0 (eye sphere, cbsp 0 / absp NULL / up to 64) and
// FCheckDprizeCollect 0x1C5B20 (prize sphere, pbspLimit 0x7FFFFFFF, one contact is enough). Steps (all proven by the draft's instruction stream):
//  1. absp == NULL: allocate min(cbspFull, 0x1555) nodes on the stack (STACKSCOPE) and PruneBsp 0x118A88 the tree to the sphere. PRUNE_Pos (wholly outside) -> 0;
//     PRUNE_Neg (wholly inside) -> step 6; else cbsp = nodes written, the last one is the root.
//  2. Every SURF of the pruned tree gets grf 0x10.
//  3. Faces: for each surface (0x10, not 0x40; sets 0x40) the segment pos + r*n .. pos - r*n is clipped to the pruned tree (ClsgClipEdgeToBsp, 16 LSGs);
//     if a piece starts/ends on this surface (apsurf[0] when au0 + au1 > 1, else apsurf[1]) the face is touched: contact kind 6
//     {pos - r*n, the face plane, 6, psurf} and grf |= 0x80.
//  4. Edges: for each surface with 0x10 and neither 0x20 nor 0x80 (sets 0x20), each edge whose other face has 0x10 but not 0x80: the point of the edge closest
//     to pos (u < 0.0001 -> vertex aipos[0], u > 0.9999 -> vertex aipos[1], else the interior point). Within the radius: unit = normalize(pos - closest),
//     plane = (unit, unit . closest) (InitPlaneFromNormalPos), point = pos - r*unit. Interior -> contact kind 5 {point, plane, 5, pedge}; a vertex -> a candidate
//     (at most 0x100 on the stack, no duplicates by vertex index).
//  5. Vertices: each candidate whose vertex is not an end of one of the edge contacts -> contact kind 4 {point, plane, 4, &geomWorld.apos[ivert]}.
//     (The game compares against the edge contacts made in step 4 only.) Reaching ccontactMax in any step skips to the clean-up.
//  6. All 0xF0 surface flags are cleared again. With no contact and the centre inside the hull (PbspPointInBsp 0x119090) the one contact comes from
//     ContactInsideBsp 0x1A9108 (our name, MATCH T1305 above: doubles a probe radius from max(r, 100) until a face plane is found, kind 0xC).
// Settled spellings, T1304 (each one moved the score): `ccontact = cvert = 0;` after the STACKSCOPE (gives `sw zero` in the ctor's delay slot); `*(qword *)&pos`
// in the PruneBsp call (address-taken param -> new pseudo, slot 0x3670 above the spills); `for (i = cbsp; --i >= 0; )` loops (no gcse of cbsp - 1); a
// function-level `BSP *pbspNode` shared by the face and edge loops (a3); a function-level `SURF *psurf` (s0 in the clean-up loop); VECTOR4 locals vecA/vecB
// before LSG alsg[16] and vecEdge/vecDelta/vecUnit/vecClosest/plane (the game's 0x3010.. slots); VERTCAND avert[0x100] before the STACKSCOPE (sp+0); QwAddScaled
// with the scale as the FIRST asm input; `-sRadius` written twice (one neg.s in f20); pposClosest starting as &apos[i0]; the dot / normalize reading
// *(qword *)&vecDelta (the vmove copy); `if (ivert >= 0) {vertex} else {edge}`; the vertex-loop bound snapshot ccontactEdge; (VECTOR4 *) apos (sll 4);
// uint cvert (sltiu); (int)grf & ~0xF0 (addiu -0xF1).
// Settled by T1341 (48% -> 99.4%; scratch tools in decomp/build/scratch/t1341: fd.py = difflib alignment, lc.sh = face-loop insn counts from -dL):
//  - The face contact's position is stored through a VU_VECTOR4 struct return (`*(VU_VECTOR4 *)&pcontact->pos = VecAddScaled(..)`): the struct copies
//    survive until the SECOND loop.c pass (97 insns there, the game needs >= 96), so 0xC / 0x60 / 1.0f stay in the face loop and &absp[i] is not reduced.
//    (Single-use copies of loop invariants die in pass 1: loop.c scan_loop substitutes them; plain inline/qword copies die in cse1.)
//  - Its stores in the order kind, plane, pv, pos; VuFloat written inside the vecA/vecB calls (each home store must precede its normal's load).
//  - ONE function-level `CMCONTACT *pcontact` for all three appends AND the vertex loop's scan of the edge contacts (`pcontact = &acontact[j]`): set twice in
//    the vertex loop, it is no induction variable there, so the game's `lw acontact; addu a2, v1, t2` (only ccontact * 0x30 reduced) appears; in the edge
//    loop it is set once and reduced (s3) as in the game.
//  - A function-level `VU_VECTOR4 posContact` for the edge/vertex point: the vertex candidate reads posContact.qw (the qmfc2 copy in v1, `sq v1`), the edge
//    contact stores the struct (`sqc2 vf2`).
//  - The interior closest point adds onto `qword posBase = *(qword *)pposClosest` (base loaded before the scale: vf1 base, vf2 scale as in the game).
//  - The squared distance reuses `u` (a multi-block pseudo -> global alloc: r*r gets f0, the distance f1 as in the game).
// Settled by T1396 (the last slot, 99.4% -> MATCH): the edge length's mtc1 (0x1A977C) sits BETWEEN the pos reload and the edge-start load in the game. The
// compiler's own mtc1 (GFromVf's float "=r" output) costs 2 cycles on the memory unit and has priority 14, so sched2 issued the edge-start load (priority 16)
// first. An asm mtc1 (no unit, 1 cycle) co-issues with the pos reload and lands in the game's slot, but its int temp is block-local: local-alloc gives it v0 and
// global-alloc then puts ppos1 in v1 (game: ppos1 v0, temp v1). The temp is therefore pinned with a register variable (`register int iLenSq asm("$3")`),
// which is what the game's allocation looks like; the other two temps (u's dot, the distance) are the compiler's mtc1 with reload registers (v1 / v0) as before.
// Natural spellings that did NOT give v1: float / int scratch, a function-level scratch, ivert / psurfOther / the index variables as the scratch (T0950 trick:
// s0 / wrong schedule), fused qmfc2+mtc1 asm.
// Gotos: the early PRUNE exits and the "contact array full" exits jump past whole phases, and the two duplicate scans branch straight to the next
// iteration of the outer loop (loop/flag/inline-function spellings all gave a peeled loop or a leftover flag test, see re/notes/camera-updatecm.md).
typedef unsigned short   uint16_t;
typedef unsigned int     uint32_t;
typedef unsigned long    uint64_t;
typedef uint16_t    ushort;
typedef uint32_t    uint;
typedef uint64_t    ulong;
typedef unsigned long long qword __attribute__((mode(TI)));
struct SO;
struct VECTOR
{
    float x, y, z;
};
struct VECTOR4
{
    float x, y, z, w;
} __attribute__((aligned(16)));
struct MATRIX4
{
	float mat[4][4];
} __attribute__((aligned(16)));
struct VU_VECTOR4
{
    qword qw;
};
struct EDGE
{
    ushort aipos[2];
    ushort aisurf[2];
    EDGE *pedgeNext;
    EDGE *pedgeOtherNext;
};
struct SURF
{
    VECTOR normal;
    float gDot;
    EDGE *pedge;
    EDGE *pedgeOther;
    uint grf;
    int ipos;
};
struct GEOM
{
      int cpos;
      VECTOR *apos;
      int csurf;
      SURF *asurf;
      int cedge;
      EDGE *aedge;
};
enum LSGK
{
};
enum PRUNE
{
    PRUNE_Neg = 0,
    PRUNE_Pos = 1,
};
struct BSP
{
    BSP *pbspNeg;
    BSP *pbspPos;
    SURF *psurf;
};
struct BSPC
{
      int cbsp;
      int cbspFull;
      int nUnk08;
      BSP *absp;
};
struct LSG
{
      qword apos[2];
      qword anormal[2];
      float au[2];
      LSGK lsgk;
      SURF *apsurf[2];
      EDGE *pedge;
      int iiposSwap;
      LSG *plsgNext;
};
int ClsgClipEdgeToBsp(BSP *pbspRoot, BSP *pbspLimit, qword pos1, qword pos2, int clsgMax, LSG *alsg);
PRUNE PruneBsp(BSP *pbsp, BSP *pbspLimit, float sRadius, qword pos, int *pcbspLeft, BSP **ppbspNext);
BSP *PbspPointInBsp(BSP *pbsp, BSP *pbspLimit, qword pos);
struct CMCONTACT
{
      qword pos;
      qword plane;
      int kind;
      void *pv;
};
enum OID
{
};
struct DLE
{
    void *next;
    void *prev;
};
struct XF;
struct SURFX;
struct LVO
{
    VECTOR *mpiposvLocal;
    VECTOR *mpiposdvLocal;
    SURFX *mpisurfsurfx;
};
struct SOPART
{
      XF *pxf;
      SO *pso;
      float gUnk08;
      OID oid;
      LVO *plvo;
      short dhp;
      ulong fUnk48 : 1;
               ulong fUnk49 : 1;
               ulong fHidden : 1;
               ulong fUnk51 : 1;
               ulong unk_0x16 : 12;
      uint grfzpk;
      int nUnk1c;
      MATRIX4 matWorld;
      DLE dle;
      BSPC bspc;
      GEOM geomLocal;
      GEOM geomWorld;
      int nStampGeom;
      int nStampWorld;
};
struct SWALLOC
{
    int cb;
    int cbAlign;
};
void *PvAllocStackImpl(SWALLOC *pswalloc);
struct STACKSCOPE
{
    STACKSCOPE();
    ~STACKSCOPE();
};
struct VU_FLOAT
{
    qword qw;
};
static inline VU_FLOAT VuFloat(float g)
{
    VU_FLOAT v;
    __asm__("qmtc2 %1, %0" : "=j"(v.qw) : "r"(g));
    return v;
}
static inline qword QwSub(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
static inline float GFromVf(qword qw)
{
    float g;
    __asm__("qmfc2 %0, %1" : "=r"(g) : "j"(qw));
    return g;
}
static inline qword QwLength(qword v)
{
    qword qw;
    qword vfOne;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qw), "=j"(vfOne) : "j"(v));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    return qw;
}
static inline qword QwDot(qword a, qword b)
{
    qword qw;
    qword vfOne;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qw), "=j"(vfOne) : "j"(a), "j"(b));
    return qw;
}
static inline qword QwNormalize(qword v)
{
    qword qw = QwLength(v);
    qword qwNormal;
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(qwNormal) : "j"(qw), "j"(v));
    return qwNormal;
}
extern "C" void InitPlaneFromNormalPos(SURF *psurf, qword qwNormal, qword qwPos);
void ContactInsideBsp(float sRadius, BSP *absp, BSP *pbspLimit, GEOM *pgeom, qword pos, CMCONTACT *pcontact);
struct VERTCAND
{
    int ivert;
    qword pos;
    qword plane;
};
static inline void *PvAllocStack(int cb)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    swalloc.cbAlign = 0x10;
    return PvAllocStackImpl(&swalloc);
}
static inline qword QwAddScaled(qword qwBase, VU_FLOAT gScale, qword qwVec)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %3, %2x" : "=j"(qw) : "j"(qwBase), "j"(gScale.qw), "j"(qwVec));
    return qw;
}
static inline VU_VECTOR4 VecAddScaled(qword qwBase, VU_FLOAT gScale, qword qwVec)
{
    VU_VECTOR4 v;
    v.qw = QwAddScaled(qwBase, gScale, qwVec);
    return v;
}
int CplaneFromSphere(qword pos, float sRadius, SOPART *ppart, BSP *pbspLimit, int cbsp, BSP *absp, int ccontactMax, CMCONTACT *acontact) __asm__("func_001A93C8");
int CplaneFromSphere(qword pos, float sRadius, SOPART *ppart, BSP *pbspLimit, int cbsp, BSP *absp, int ccontactMax, CMCONTACT *acontact)
{
    int ccontact;
    uint cvert;
    BSP *pbsp;
    int cbspLeft;
    SURF *psurf;
    BSP *pbspNode;
    VU_VECTOR4 posContact;
    CMCONTACT *pcontact;
    VERTCAND avert[0x100];
    STACKSCOPE scope;
    ccontact = cvert = 0;
    int ccontactFace;
    int ccontactEdge;
    if (ccontactMax > 0)
    {
        if (absp == 0)
        {
            int cbspAlloc = 0x1555;
            if (ppart->bspc.cbspFull < 0x1555)
                cbspAlloc = ppart->bspc.cbspFull;
            cbspLeft = cbspAlloc;
            absp = (BSP *)PvAllocStack(cbspAlloc * sizeof(BSP));
            pbsp = absp;
            PRUNE prune = PruneBsp(ppart->bspc.absp, pbspLimit, sRadius, *(qword *)&pos, &cbspLeft, &pbsp);
            if (prune == PRUNE_Neg)
                goto inside;
            if (prune == PRUNE_Pos)
                goto done;
            cbsp = pbsp - absp;
        }
        int i;
        pbsp = absp + (cbsp - 1);
        for (i = cbsp; --i >= 0; )
            absp[i].psurf->grf |= 0x10;
        for (i = cbsp; --i >= 0; )
        {
            pbspNode = &absp[i];
            psurf = pbspNode->psurf;
            if (psurf->grf & 0x10)
            {
                if (!(psurf->grf & 0x40))
                {
                    psurf->grf |= 0x40;
                    VECTOR4 vecA;
                    VECTOR4 vecB;
                    LSG alsg[16];
                    *(qword *)&vecA = QwAddScaled(pos, VuFloat(sRadius), *(qword *)pbspNode->psurf);
                    *(qword *)&vecB = QwAddScaled(pos, VuFloat(-sRadius), *(qword *)pbspNode->psurf);
                    int clsg = ClsgClipEdgeToBsp(pbsp, (BSP *)0x7fffffff, *(qword *)&vecA, *(qword *)&vecB, 16, alsg);
                    int ilsg;
                    for (ilsg = 0; ilsg < clsg; ilsg++)
                    {
                        SURF *psurfHit;
                        if (1.0f < alsg[ilsg].au[0] + alsg[ilsg].au[1])
                            psurfHit = alsg[ilsg].apsurf[0];
                        else
                            psurfHit = alsg[ilsg].apsurf[1];
                        if (psurfHit == psurf)
                            break;
                    }
                    if (ilsg < clsg)
                    {
                        if (ccontact < ccontactMax)
                        {
                            pcontact = &acontact[ccontact++];
                            qword plane = *(qword *)psurf;
                            pcontact->kind = 6;
                            pcontact->plane = plane;
                            pcontact->pv = psurf;
                            *(VU_VECTOR4 *)&pcontact->pos = VecAddScaled(pos, VuFloat(-sRadius), plane);
                            if (ccontact >= ccontactMax)
                                goto clear;
                        }
                        psurf->grf |= 0x80;
                    }
                }
            }
        }
        ccontactFace = ccontact;
        for (i = cbsp; --i >= 0; )
        {
            VECTOR4 *apos = (VECTOR4 *)ppart->geomWorld.apos;
            SURF *asurf = ppart->geomWorld.asurf;
            pbspNode = &absp[i];
            psurf = pbspNode->psurf;
            if (psurf->grf & 0x10)
            {
                if (!(psurf->grf & 0xA0))
                {
                    psurf->grf |= 0x20;
                    for (EDGE *pedge = psurf->pedge; pedge != 0; pedge = pedge->pedgeNext)
                    {
                        SURF *psurfOther = &asurf[pedge->aisurf[1]];
                        if (!(psurfOther->grf & 0x10))
                            continue;
                        if (psurfOther->grf & 0x80)
                            continue;
                        VECTOR4 vecEdge;
                        VECTOR4 vecDelta;
                        VECTOR4 vecUnit;
                        VECTOR4 vecClosest;
                        VECTOR4 plane;
                        VECTOR4 *pposClosest = &apos[pedge->aipos[0]];
                        VECTOR4 *ppos1 = &apos[pedge->aipos[1]];
                        int ivert;
                        *(qword *)&vecEdge = QwSub(*(qword *)ppos1, *(qword *)pposClosest);
                        register int iLenSq __asm__("$3");
                        float gLenSq;
                        __asm__("qmfc2 %0, %1" : "=r"(iLenSq) : "j"(QwDot(*(qword *)&vecEdge, *(qword *)&vecEdge)));
                        __asm__("mtc1 %1, %0" : "=f"(gLenSq) : "r"(iLenSq));
                        *(qword *)&vecDelta = QwSub(pos, *(qword *)pposClosest);
                        float u = GFromVf(QwDot(*(qword *)&vecDelta, *(qword *)&vecEdge)) / gLenSq;
                        if (u < 0.0001f)
                        {
                            ivert = pedge->aipos[0];
                        }
                        else if (0.9999f < u)
                        {
                            pposClosest = ppos1;
                            ivert = pedge->aipos[1];
                            *(qword *)&vecDelta = QwSub(pos, *(qword *)pposClosest);
                        }
                        else
                        {
                            ivert = -1;
                            qword posBase = *(qword *)pposClosest;
                            *(qword *)&vecClosest = QwAddScaled(posBase, VuFloat(u), *(qword *)&vecEdge);
                            pposClosest = &vecClosest;
                            *(qword *)&vecDelta = QwSub(pos, *(qword *)&vecClosest);
                        }
                        u = GFromVf(QwDot(*(qword *)&vecDelta, *(qword *)&vecDelta));
                        if (u > sRadius * sRadius)
                            continue;
                        {
                            if (ivert >= 0)
                            {
                                for (int j = cvert - 1; j >= 0; j--)
                                    if (avert[j].ivert == ivert)
                                        goto nextedge;
                            }
                            *(qword *)&vecUnit = QwNormalize(*(qword *)&vecDelta);
                            InitPlaneFromNormalPos((SURF *)&plane, *(qword *)&vecUnit, *(qword *)pposClosest);
                            posContact = VecAddScaled(pos, VuFloat(-sRadius), *(qword *)&vecUnit);
                            if (ivert >= 0)
                            {
                                if (cvert < 0x100)
                                {
                                    VERTCAND *pvert = &avert[cvert++];
                                    pvert->ivert = ivert;
                                    pvert->pos = posContact.qw;
                                    pvert->plane = *(qword *)&plane;
                                }
                            }
                            else
                            {
                                if (ccontact >= ccontactMax)
                                    continue;
                                pcontact = &acontact[ccontact++];
                                *(VU_VECTOR4 *)&pcontact->pos = posContact;
                                pcontact->plane = *(qword *)&plane;
                                pcontact->kind = 5;
                                pcontact->pv = pedge;
                                if (ccontact >= ccontactMax)
                                    goto clear;
                            }
                        }
nextedge:
                        ;
                    }
                }
            }
        }
        ccontactEdge = ccontact;
        for (int iv = cvert - 1; iv >= 0; iv--)
        {
            int ivert = avert[iv].ivert;
            for (int j = ccontactFace; j < ccontactEdge; j++)
            {
                pcontact = &acontact[j];
                EDGE *pedge = (EDGE *)pcontact->pv;
                if (pedge->aipos[0] == ivert || pedge->aipos[1] == ivert)
                    goto nextvert;
            }
            if (ccontact < ccontactMax)
            {
                pcontact = &acontact[ccontact++];
                pcontact->pos = avert[iv].pos;
                pcontact->plane = avert[iv].plane;
                pcontact->kind = 4;
                pcontact->pv = &((VECTOR4 *)ppart->geomWorld.apos)[ivert];
                if (ccontact >= ccontactMax)
                    break;
            }
nextvert:
            ;
        }
clear:
        for (i = cbsp; --i >= 0; )
        {
            psurf = absp[i].psurf;
            psurf->grf = (int)psurf->grf & ~0xF0;
        }
inside:
        if (ccontact == 0 && PbspPointInBsp(ppart->bspc.absp, pbspLimit, pos) != 0)
        {
            ccontact = 1;
            ContactInsideBsp(sRadius, ppart->bspc.absp, pbspLimit, &ppart->geomWorld, pos, acontact);
        }
    }
done:
    return ccontact;
}
