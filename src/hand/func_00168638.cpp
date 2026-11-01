// func_00168638 (Claude agent, lane Z1, 2026-10-30): MATCH, 728/728, frame 0x290. Source = build/hand_wip/func_00168638.lilly5.cpp unchanged.
//   Policy: no GPR/FPU pins. The only register bindings are the microprogram 0x1C0 I/O registers (vf10..vf19) inside the
//   inline GCullSphere/GCullSphereSync helpers (allowed: VU microprogram registers bound inside an inline helper). All other asms
//   are VU helper inlines (lw/qmfc2/mtc1 VU_FLOAT-style, as in Lilly's files) or commented empty asms; globals are D_ names only.
// lilly5 (Claude agent, T1703, 2026-10-26): MATCH (728/728, frame 0x290). NOT in src/hand only because of the team's one-pin rule:
//   the microprogram 0x1C0 inputs/outputs are register variables bound to vf10..vf19 (20 `register ... __asm__("vfN")`, as in
//   func_001684D0.c / CblipeProjectBlipTransformAccel lilly1.c); the orchestrator counts each as a pin. No GPR/FPU pins.
//   From lilly3 (aligned 2), cull 2 (GCullSphereSync) only: (1) qmfc2 straight into a float "=r" (the qmfc2 output becomes a reload
//   in v0, reload's copy is the mtc1 v0,f0, so the i+1 reload after it gets v1 by round robin); (2) cfc2 outputs only vf19, an empty
//   `"=j"(qwDpos)` asm declares vf18, and one empty `"+j"(qwDpos)` asm with a dead "$3" clobber: two no-code insns ready right after
//   the cfc2 fill sched1's free slots (the PRE i+1 insn now issues after the swc1 gVisSub), and the $3 clobber holds the addiu v1
//   in sched2 until after it -> target `c.eq.s; swc1; bc1t; sw v1,0x1A4`. The same with two "+j" asms also MATCHes.
// (probe) lilly4 (Claude agent, T1700, 2026-10-26): PROBE, aligned 4 (lilly3 = 2 stays the best score), 728 instructions, frame 0x290, no pins.
//   Shows the mechanism behind lilly3's last diff (cull-2 tail sw v1,0x1A4 / swc1 f0,0x158). Proven with -fno-schedule-insns: if sched1
//   leaves the i+1 PRE insn AFTER the gVisSub store, sched2 + reorg give the target's swc1; bc1t; sw. In sched1 that insn is ready right
//   after the volatile vcallms and takes any free second issue slot (t13, t14), so it needs two insns ready there that vanish later.
//   Here: two empty "+j"(qwDpos) asms after the cfc2 (no code) -> the store order is now the target's. Left (4, registers only):
//   qmfc2/mtc1 use a0 (target v0) and the i+1 reload uses v0 (target v1). Without the "$2" clobber on the first filler n is v0 but the
//   i+1 reload also takes v0 (reload round-robin: the target's last reload before it must have used v0; a qmfc2 "=r,m" output into
//   *(int *)&gVisSub gives v1 but leaves a dead sw v0,0x158). Not a source form to keep: it is a map for the next attempt.
// lilly3 (Claude agent, T1698, 2026-10-26): aligned 2 (lilly2 11), 728 instructions, frame 0x290, no pins. Cull 1 and the cull-2 body MATCH.
//   Key: the target's three HOME stores (sq r,0(sp) / 0x50(sp)) are the asm-output temps of the VU_FLOATM ctors (MEM_SCALAR_P at a fixed
//   frame address in sched1). gcc 2.95's fixed_scalar_and_varying_struct_p lets a load whose MEM is a struct member at a varying address
//   pass them, so the range is read from the SPH itself (VU_FLOATS, "m"(psph->sRange)) LAST in source order; sched1 hoists the load
//   above the gain/radius HOME stores while its HOME store stays last. OBJ+0x28 is an embedded SPH (pos[3], rad, sRange at OBJ+0x38),
//   so both culls pass a const SPH *. Gain is VU_FLOATM too (all three are lw asms: unit none, 1-cycle links -> target issue order).
//   Cull 2 = combined-body loads + split vcallms/cfc2 + int qmfc2 ($3 clobber) + mtc1 asm.
//   Left (2): cull-2 tail `swc1 f0,0x158` / `sw v1,0x1A4` (sched2 tie prio 2/class 3/1 dep -> LUID; sched1 issues the i+1 PRE insn 2417
//   in the free slot at t13, before the gVisSub store, so reload's spill sw precedes swc1). Needs two more ready insns after cfc2 in sched1.
// lilly2 (Claude agent, T1691, 2026-10-26): aligned 11 (lilly1 14), 728 instructions, frame 0x290, no pins.
//   Cull 2's range is now a VU_FLOATM (asm lw) built at the call site and passed as a qword: the range load stays first and the
//   registers are the target's (range a0, radius v1, gain v0; one pseudo for range instead of the lw + tied empty asm qty,
//   so it no longer outranks the radius in local-alloc). Left: (a) cull 1 load order/registers (unchanged from lilly1);
//   (b) cull 2 range HOME sq now issues FIRST (the call-site temp is stored at the call), target stores gain, radius, range;
//   (c) cull-2 tail sw v1,0x1A4 / swc1 f0,0x158 (sched2 tie: both prio 2, class 3, 1 dependent; the reload spill's LUID wins
//   because sched1 issues the i+1 PRE addiu at t14, two cycles before the swc1).
// lilly1 (Claude agent, T1687, 2026-10-25): aligned 14 (728 instructions, frame 0x290; laneH21v2 30). Policy-clean: no register pins, VU helper asms only.
//   From laneH21v2: (1) cull 2 radius loaded by a `lw` asm straight into the qword (VU_FLOATM, as func_001375D0): real-load timing, target order (30 -> 28).
//   (2) case-3 SqDist: the compiler moves the length out of vf (qmfc2) and an `mtc1` asm makes the float (GetX idiom, func_001D2490): mtc1 before add.s (28 -> 26).
//   (3) cull-2 result: qmfc2 asm into an int (dead-$3 clobber kept) + separate `mtc1` asm: c.eq.s/swc1 one cycle earlier, the hazard nop and the 1-insn shift are gone (26 -> 15).
//   (4) UnpackWords copy as `for (k = 0; k < cw; k++)`: loop.c creates the counter init after gcse's &matModel PRE insn, as func_00169198 (15 -> 14).
//   Left: cull 1 load/sq order + registers (target: radius lw after gain, regs v1/a0/v0/a2), cull 2 radius/range registers swapped (target radius v1, range a0),
//   cull-2 tail `sw v1,0x1A4` / `swc1 f0,0x158` (sched1 puts the i+1 PRE insn before the swc1, so reload's spill store wins the sched2 luid tie).
// laneH21v2 (Claude agent): aligned 30. laneI9v1 + CopyQws as `do { } while (0);` + for-k copy loop (no asm barrier; 31) + cull 2 through GCullSphereSync (vcallms and the cfc2.i sync as separate asms: target mtc1 zero / qmfc2 order) with a commented dead-$3 clobber on its qmfc2 (30).
// laneI9v1 (Claude agent): aligned 31. laneI8v1 + final dist qmfc2 written straight to prpl->dist via "=r,m" : "j,!r" (never-chosen m alternative keeps the MEM; the qmfc2 output reload in v0 advances reload's round robin, so the pbStart reload gets v1 as in the target).
// laneI8v1 (Claude agent): aligned 33. laneI7v2 + KindOf as (ulong)(psub->grf & 0xF8) >> 3 (target lbu + dsrl in the final switch, 35 -> 33).
// laneI7v2 (Claude agent): aligned 35. laneI6v2 + params (pmodel, pmatWorld, gFirst, gSecond, pshd) (entry pshd spill order, 51 -> 48) + case-3 SqDist returns float straight from qmfc2 (48 -> 35).
// laneI6v2 (Claude agent): aligned 51. laneI5v4 + final GBitsSqLength with qwOne as operand 0, case 3 GBitsSqDist (old order), fAlt kind as (x != 1.0f) ? 23 : 22.
// laneI5v4 (Claude agent): aligned 57. laneI4v5 + pad loop via `DMA *pdma` and a `pbPad` cursor, skin offset `int ibMat = imat*0x40 + 0x6F0`, commented empty volatile asm at the start of CopyQws.
// func_00168638 (Claude agent, lane I3): fresh rewrite in the style of Lilly's matched files.
// laneI4v5 (Claude agent): aligned 120 (raw 179), frame 0x290. v4 + posCam, pos, sRadius declaration order.
// Culls a model against the view (microprogram 0x1C0), then builds the VIF packet for every visible submodel
// (optional reference, model matrix, params, bone matrices (0x1684D0), skin matrices) and queues it for rendering.
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
struct SPH
{
    float pos[3];
    float rad;
    float sRange;
    int pad;
};
struct OBJ
{
    char pad[0xC];
    VTENT *pvt;
    char pad10[0x18];
    SPH sph;
    char pad40[0x20];
    XF *pxf;
};
struct BONE;
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
struct MODEL
{
    OBJ *powner;
    float t;
    char pad8[8];
    VECTOR4 posCenter;
    float rad;
    int cobj;
    OBJ **apobj;
    char pad2C[8];
    uint grfMode;
    int csub;
    SUBDESC *asubdesc;
    SUB *asub;
    SPH *asph;
    int pad48;
    int nStamp;
    union
    {
        ulong grf;
        float scale;
    };
};
struct SHD
{
    uint aw[9];
    MATRIX4 *pmatClip;
    uint ibRef;
    uint cqwRef;
};
struct VIEW
{
    char pad[0x1C];
    float sGain;
    char pad20[0x28];
    float sNear;
    char pad4C[0x44];
    qword posCam;
    qword aqwPlane[3];
};
struct CLOCK
{
    float t, dt;
};
struct G
{
    char pad[0x3BEC];
    int fAlt;
    char pad3BF0[0x16C];
    int nStamp;
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
extern DMA D_002DEA28;
extern G D_003DD490;
extern VIEW D_002DDF40;
extern CLOCK D_002DDED0;
extern MATRIX4 D_002A2D30;
extern "C" {
void func_001681F8(MODEL *pmodel);
int func_001EDA20(OBJ *pobj);
void func_001AE5A0(void *pvDst, const void *pvSrc, uint cb);
void *func_0023320C(void *pvDst, const void *pvSrc, uint cb);
void __ml__FRC7MATRIX4T0(MATRIX4 *pmatDst, const MATRIX4 *pmatA, const MATRIX4 *pmatB);
void InvertMatrix4Rigid(MATRIX4 *pmatDst, const MATRIX4 *pmatSrc);
float func_0011CAB0(const MATRIX4 *pmat);
qword func_00167DA0(const MATRIX4 *pmat, qword qwPos, float *psRadius);
int func_0014F898(VIEW *pview, qword qwDpos, float sRange, float *pgVis);
void func_001684D0(MODEL *pmodel, SUBDESC *pdesc, SUB *psub);
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
static inline qword QwXfmRef(const MATRIX4 &mat, const qword &qwPos)
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
    // the "m" / "!r" alternative is allocation-only (never chosen): it keeps the destination a MEM, so the qmfc2 result is an output reload
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
static inline float SqDist(qword qw0, qword qw1)
{
    qword qwV = QwSub(qw0, qw1);
    qword qwSq, qwOne;
    float g;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(qwSq), "=&j"(qwOne) : "j"(qwV));
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(qwSq));
    return g;
}
// the word loaded straight into the qword's register (as func_001375D0's VU_FLOATM)
struct VU_FLOATM
{
    qword qw;
    VU_FLOATM(const float &g) { __asm__("lw %0, %1" : "=r"(qw) : "m"(g)); }
};
// The sphere's range read straight from the SPH: the struct-member MEM (MEM_IN_STRUCT_P, varying address) does not alias the
// scalar VU_FLOAT temp HOME at a fixed frame address, so sched1 hoists this load above the gain/radius HOME stores (target order).
struct VU_FLOATS
{
    qword qw;
    VU_FLOATS(const SPH *psph) { __asm__("lw %0, %1" : "=r"(qw) : "m"(psph->sRange)); }
};
// Microprogram 0x1C0 (sphere against the view planes): its inputs and outputs live in fixed VU registers.
static inline float GCullSphere(qword posCam, qword pos, const SPH *psph, const float &sRadius, qword *pdpos)
{
    qword *aqwPlane = D_002DDF40.aqwPlane;
    register qword qwPlane0 __asm__("vf10") = aqwPlane[0];
    register qword qwPlane1 __asm__("vf11") = aqwPlane[1];
    register qword qwPlane2 __asm__("vf12") = aqwPlane[2];
    register qword qwCam __asm__("vf13") = posCam;
    register qword qwGain __asm__("vf14") = VU_FLOATM(D_002DDF40.sGain).qw;
    register qword qwPos __asm__("vf15") = pos;
    register qword qwRadius __asm__("vf16") = VU_FLOATM(sRadius).qw;
    register qword qwRange __asm__("vf17") = VU_FLOATS(psph).qw;
    register qword qwDpos __asm__("vf18");
    register qword qwVis __asm__("vf19");
    __asm__ __volatile__("vcallms 0x1C0\n\tcfc2.i $zero, $vi0"
                         : "=j"(qwDpos), "=j"(qwVis)
                         : "j"(qwPlane0), "j"(qwPlane1), "j"(qwPlane2), "j"(qwCam), "j"(qwGain), "j"(qwPos), "j"(qwRadius), "j"(qwRange)
                         : "vf20", "vf21", "vf22", "vf23", "vf24", "vf25");
    *pdpos = qwDpos;
    float g;
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(qwVis));
    return g;
}
// Same cull, with the VU0 sync as its own statement and the visibility moved out through a GPR.
static inline float GCullSphereSync(qword posCam, qword pos, const SPH *psph, const float &sRadius, qword *pdpos)
{
    qword *aqwPlane = D_002DDF40.aqwPlane;
    register qword qwPlane0 __asm__("vf10") = aqwPlane[0];
    register qword qwPlane1 __asm__("vf11") = aqwPlane[1];
    register qword qwPlane2 __asm__("vf12") = aqwPlane[2];
    register qword qwCam __asm__("vf13") = posCam;
    register qword qwGain __asm__("vf14") = VU_FLOATM(D_002DDF40.sGain).qw;
    register qword qwPos __asm__("vf15") = pos;
    register qword qwRadius __asm__("vf16") = VU_FLOATM(sRadius).qw;
    register qword qwRange __asm__("vf17") = VU_FLOATS(psph).qw;
    register qword qwDpos __asm__("vf18");
    register qword qwVis __asm__("vf19");
    __asm__ __volatile__("vcallms 0x1C0" : : "j"(qwPlane0), "j"(qwPlane1), "j"(qwPlane2), "j"(qwCam), "j"(qwGain), "j"(qwPos), "j"(qwRadius), "j"(qwRange) : "vf18", "vf19", "vf20", "vf21", "vf22", "vf23", "vf24", "vf25");
    // wait for the microprogram (cfc2 interlocks on it); vf19 = the visibility
    __asm__("cfc2.i $zero, $vi0" : "=j"(qwVis));
    // vf18 = the camera-to-sphere vector the microprogram left behind (no code)
    __asm__("" : "=j"(qwDpos));
    // scheduling-only barrier (no code): with the empty asm above it fills sched1's free issue slots after the cfc2, so the
    // i+1 PRE insn issues after the gVisSub store; its dead $3 clobber keeps the i+1 addiu (v1) after it in sched2
    __asm__("" : "+j"(qwDpos) : : "$3");
    *pdpos = qwDpos;
    // float "=r" output: the qmfc2 goes to a reload register (v0) and reload's copy is the mtc1 v0,f0 (next reload: v1)
    float g;
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(qwVis));
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
static inline void UnpackWords(uint tag, const void *pv, int cw)
{
    DMA *pdma = &D_002DEA28;
    *(uint *)pdma->pb = tag;
    pdma->pb += 4;
    uint *pwDst = (uint *)pdma->pb;
    const uint *pwSrc = (const uint *)pv;
    for (int k = 0; k < cw; k++)
        *pwDst++ = *pwSrc++;
    pdma->pb += cw * 4;
}
static inline int KindOf(SUB *psub)
{
    return (ulong)(psub->grf & 0xF8) >> 3;
}
extern "C" void func_00168638(MODEL *pmodel, MATRIX4 *pmatWorld, float gFirst, float gSecond, SHD *pshd)
{
    SUBDESC *pdesc;
    qword posCam = D_002DDF40.posCam;
    qword pos = QwXfmRef(pmodel->powner->pxf->mat, *(qword *)&pmodel->posCenter);
    float sRadius = pmodel->rad;
    if (pmatWorld)
    {
        pos = QwXfm(*pmatWorld, pos);
        sRadius *= func_0011CAB0(pmatWorld);
    }
    if ((pmodel->grf & 0x200000000UL) && pshd->pmatClip)
        pos = func_00167DA0(pshd->pmatClip, pos, &sRadius);
    qword dpos;
    float gVis = GCullSphere(posCam, pos, &pmodel->powner->sph, sRadius, &dpos);
    if (gVis == 0.0f)
        return;
    pmodel->t = D_002DDED0.t;
    if (pmodel->nStamp != D_003DD490.nStamp)
        func_001681F8(pmodel);
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
    int fAlt = 0;
    if (pmodel->grf & 0x400000000UL)
        fAlt = D_003DD490.fAlt == 0;
    for (i = 0; i < pmodel->csub; i++)
    {
        pdesc = &pmodel->asubdesc[i];
        SUB *psub = &pmodel->asub[i];
        SPH *psph = &pmodel->asph[i];
        VECTOR4 posSph;
        MATRIX4 matModel;
        if (psub->grf & 0x800)
            continue;
        if (psub->grf & 0x1000)
        {
            func_001AE5A0(&matModel, psub->pb + 0x14, sizeof(MATRIX4));
            if (pmatWorld)
                matModel = MulMat(*pmatWorld, matModel);
        }
        else
        {
            MATRIX4 *pmatParent = psub->pmatParent;
            MATRIX4 mat;
            CopyMat(mat, psub->pxf->mat);
            if (pmatParent)
                CopyMat(mat, MulMat(mat, *pmatParent));
            if (pmatWorld)
                CopyMat(mat, MulMat(*pmatWorld, mat));
            matModel = mat;
        }
        func_001AE5A0(&posSph, psph, 12);
        qword posSub = QwXfm(matModel, *(qword *)&posSph);
        float sRadiusSub = func_0011CAB0(&matModel) * psph->rad;
        if ((psub->grf & 7) == 2 && pshd->pmatClip)
            posSub = func_00167DA0(pshd->pmatClip, posSub, &sRadiusSub);
        float gVisSub;
        qword dposSub;
        if (!psub->abone)
        {
            qword dposCull;
            gVisSub = GCullSphereSync(posCam, posSub, psph, sRadiusSub, &dposCull);
            if (gVisSub == 0.0f)
                continue;
            dposSub = dposCull;
        }
        else
        {
            dposSub = QwSub(posSub, posCam);
            if (!func_0014F898(&D_002DDF40, dposSub, psph->sRange, &gVisSub))
                continue;
        }
        uchar *pbStart = D_002DEA28.pb;
        if (psub->grf & 0x100)
        {
            *(ulong *)pbStart = (ulong)(int)(pshd->aw[5] | 0x40000000) | ((ulong)(int)pshd->aw[6] << 32);
            D_002DEA28.pb += 0x10;
        }
        ulong *ptag;
        switch (psub->grf & 7)
        {
        case 2:
            if (!pshd->cqwRef)
                continue;
            *(ulong *)D_002DEA28.pb = (ulong)(int)(pshd->ibRef | 0x40000000) | ((ulong)pshd->cqwRef << 32);
            D_002DEA28.pb += 0x10;
            ptag = (ulong *)D_002DEA28.pb;
            *ptag = 0x10000000;
            D_002DEA28.pb += 0x10;
            break;
        case 3:
        {
            ptag = (ulong *)D_002DEA28.pb;
            *ptag = 0x10000000;
            float g = SqDist(posCam, posSub);
            float sNear = sRadiusSub + D_002DDF40.sNear;
            D_002DEA28.pb += 0x10;
            int fNear = g < sNear * sNear;
            UnpackWords(0x6001C012, &fNear, 1);
            break;
        }
        default:
            ptag = (ulong *)D_002DEA28.pb;
            *ptag = 0x10000000;
            D_002DEA28.pb += 0x10;
            break;
        }
        if (psub->abone)
        {
            MATRIX4 *pmat = pmatWorld;
            if (!pmat)
                pmat = &D_002A2D30;
            UnpackQws<0x6C04C000>(pmat, 16);
        }
        else
        {
            UnpackQws<0x6C04C000>(&matModel, 16);
        }
        VECTOR4 vecParams;
        if (gVis < gVisSub)
        {
            vecParams.x = gFirst * gVis;
            vecParams.y = gSecond * gVis;
        }
        else
        {
            vecParams.x = gFirst * gVisSub;
            vecParams.y = gSecond * gVisSub;
        }
        vecParams.z = pmodel->scale;
        *(uint *)&vecParams.w = (psub->grf & 0x200) ? pmodel->grfMode : 0;
        UnpackQws<0x6C01C008>(&vecParams, 4);
        if (psub->abone)
            func_001684D0(pmodel, pdesc, psub);
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
                    int ibMat = imat * 0x40 + 0x6F0;
                    mat = MulMat(matInv, *(MATRIX4 *)(pskin->pbOwner + ibMat));
                    UnpackQws(ib | 0x6C04C000, &mat, 16);
                    ib += 4;
                }
                if (pskin->fSecond)
                {
                    UnpackQws(ib | 0x6C04C000, pskin->pbOwner + 0x7F0 + imat * 0x40, 16);
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
        switch (KindOf(psub))
        {
        case 3:
        case 4:
            if (fAlt)
                prpl->kind = (vecParams.x != 1.0f) ? 23 : 22;
            else if (vecParams.x == 1.0f)
                prpl->kind = KindOf(psub);
            else
                prpl->kind = ((psub->grf & 0xF8) == 0x18) ? 5 : 6;
            break;
        case 5:
        case 6:
            prpl->kind = fAlt ? 23 : KindOf(psub);
            break;
        case 9:
            prpl->kind = fAlt ? 24 : KindOf(psub);
            break;
        case 10:
            prpl->kind = fAlt ? 25 : KindOf(psub);
            break;
        default:
            prpl->kind = KindOf(psub);
            break;
        }
        if (psub->grf & 0x400)
            prpl->dist = pdesc->dist;
        else
            GBitsSqLengthTo(*(int *)&prpl->dist, dposSub);
        prpl->pb = pbStart;
        prpl->pv = psub->pb;
    }
}
