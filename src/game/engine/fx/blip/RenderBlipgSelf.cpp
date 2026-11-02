// RenderBlipgSelf: light-list VU upload (Claude agent for Lilly, T1676; built on the team's v2..v14 drafts).
// Collects the lights of p->xE0's list whose position (PosLoWorld 0x1A62E8 + PmatLoWorld 0x1A6318 rotating p->vF0)
// passes the camera tests FInsideCmFrustum / FInsideCmMrd, as M4s in a stack array (or the single D_002A2D30 when the
// list check fails), then uploads each light per DMA queue through the VIF helpers.
// Matching notes (gather loop, the last diffs; found with sched1/sched2/local-alloc dumps):
//  * The g100 lw must issue after the &D_002DDF40 addiu in sched1 (else lw and lui overlap and the lui gets v1):
//    the lw is written inside the VU_FLOAT asm with an allocation-only "r"(pcm) input.
//  * The vF0 barrier is an operand-less volatile asm: a "+j"(v) one gives v 2 more refs, and local-alloc then
//    ranks the vF0 chain above mat.r[2] (vF0 vf1 instead of vf2).
//  * Plain Sub (2 refs on pcm->pos): with SubE's duplicate operand the cm pos qty outranks the vF0 chain (vf2/vf3 swap).
//  * The empty "=r"(z) asm only exists for sched2 (see the comment there).
typedef int qword __attribute__((mode(TI)));
struct CM;
struct M3 { qword r[3]; };
struct M4 { qword r[3]; float x30, y30, z30, w30; };
struct FRAME { int a[4]; };
struct AL { int cb; int align; AL(int c, int a) { cb = c; align = a; } };
struct VU_FLOAT {
    qword q;
    /* else branch: word move into a 128-bit GPR value; volatile keeps the g100 load before the vsub like the target */
    VU_FLOAT(float f) { __asm__ volatile("" : "=r"(q) : "0"(*(int *)&f)); }
    /* gather loop: the lw itself, with an allocation-only "r"(pcm) so it can only issue after &D_002DDF40 is built
       (sched1 then keeps the lw and the lui apart, and both get v0 like the target) */
    VU_FLOAT(const float *pf, CM *pcm) { __asm__("lw %0, %1" : "=r"(q) : "m"(*pf), "r"(pcm)); }
};
struct DL { void *head; void *tail; int ibDle; };
struct LO;
struct EX { int pad[2]; struct LO **pplo; };
struct LO { char pad[0x10]; LO *x10; char pad14[0x38 - 0x14]; float g38; char pad3C[0xE4 - 0x3C]; EX *xE4; char padE8[0x11C - 0xE8]; LO *x11C; char pad120[8]; LO *x128; };
struct BUF { qword a[0x2B]; qword b[0x2B]; qword c[0x2B]; char pad[0x840 - 0x810]; };
struct Q { BUF abuf[2]; int i; int c; int pad[2]; Q *pqNext; };
struct P { char pad[0xE0]; LO *xE0; char padE4[0xF0 - 0xE4]; qword vF0; float g100; char pad104[0x43C - 0x104]; int x43C; int pad440[2]; Q *pqFirst; };
struct DMA { int x0; int x4; int x8; int xC; };
struct VIFS { int pad[3]; int xC; };
struct CM { char pad[0x90]; qword pos; };
extern "C" {
extern M3 D_002A2D00;
extern M4 D_002A2D30;
extern CM D_002DDF40;
extern VIFS D_002DEA28;
extern char D_00000000[];
extern char D_00000390[];
extern char D_00000870[];
void STACKSCOPE_ctor(FRAME *pf);
void STACKSCOPE_ctor_1AE4A0(FRAME *pf, int n);
int FIsDlEmpty__FP2DL(void *pdl);
int CPvDl__FP2DL(void *pdl);
void *func_001AE318(const AL &al);
int func_001A61A0(LO *plo);
qword func_001A62E8(LO *plo);
M3 *func_001A6318(LO *plo);
int FInsideCmFrustum(CM *pcm, qword d, VU_FLOAT r);
int FInsideCmMrd(CM *pcm, qword d, float g, float *pg);
void VECTOR_opAssign(void *pv, qword v);
DMA *PrplAllocRpl(void);
void AddDmaCnt__4DMAS(VIFS *p);
void func_0013A688(P *p);
void AddVifUnpack__4VIFS3UPKiPvi(VIFS *p, ...);
void AddVifStcycl__4VIFSii(VIFS *p, int a, int b);
void AddVifUnpackRefs__4VIFS3UPKiPviPPPv(VIFS *p, int a, int c, void *pv, int addr, int flag);
void AddVifMscal__4VIFSPv(VIFS *p, void *pv);
void AddVifFlush__4VIFS(VIFS *p);
void EndDmaCnt__4DMAS(VIFS *p);
void AddDmaRet__4DMAS(VIFS *p);
void RenderBlipgSelf(P *p);
}
static inline void *PvAllocStack(int cb) { return func_001AE318(AL(cb, 0x10)); }
static inline qword Sub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
/* earlyclobber output (else branch: vsub vf3 distinct from its inputs); the 2nd "j"(b) is allocation-only */
static inline qword SubE(qword a, qword b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=&j"(r) : "j"(a), "j"(b), "j"(b)); return r; }
/* "r"(z) inputs below are allocation-only: they only add scheduler dependences (see the comment in the gather loop) */
static inline qword AddZ(qword a, qword b, int z) { qword r; __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b), "r"(z)); return r; }
static inline qword RotKZ(const M3 &m, qword v, qword k, int z)
{
    qword r;
    /* "r"(k) (the VU_FLOAT) keeps the lw on the Rot's critical path, like the target's sched2 order */
    __asm__("vmulax.xyz $ACC, %1, %0x\n\tvmadday.xyz $ACC, %2, %0y\n\tvmaddz.xyz %0, %3, %0z" : "=j"(r) : "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]), "0"(v), "r"(k), "r"(z));
    return r;
}

/** @sly2-annot addr=0x13A1E8
 * Purpose: BLIPG vt19 RenderSelf: build VU1 DMA for the blip group
 * Info: proposed name RenderBlipgSelf (T1); class BLIPG slot19; TU blip
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/particles-effects.md#6-key-functions, docs/engine/particles-effects.md#open-questions,
 *   docs/engine/rendering.md#2-render-list-entries-rpl
 * @end-sly2-annot */
void RenderBlipgSelf(P *p)
{
    FRAME frame;
    STACKSCOPE_ctor(&frame);
    int cLight;
    M4 *aLight;
    if (p->xE0 != 0 && FIsDlEmpty__FP2DL(&p->xE0->x11C) == 0) {
        cLight = 0;
        aLight = (M4 *)PvAllocStack(CPvDl__FP2DL(&p->xE0->x11C) << 6);
        M4 *pl = aLight;
        LO *plo = p->xE0->x11C;
        M3 *pmId;
        if (!plo) goto done;
        pmId = &D_002A2D00;
    top:
            if (!func_001A61A0(plo))
                goto next;
            {
            LO **pplo = plo->xE4->pplo;
            qword pos;
            M3 mat;
            if (*pplo) {
                pos = func_001A62E8(*pplo);
                mat = *func_001A6318(*pplo);
            } else {
                pos = func_001A62E8(plo);
                if (plo->x10)
                    mat = *func_001A6318(plo->x10);
                else
                    mat = *pmId;
            }
            qword v = p->vF0;
            __asm__ volatile(""); /* barrier: the vF0 load issues first (an operand-less one adds no refs to v) */
            CM *pcm = &D_002DDF40;
            VU_FLOAT vf(&p->g100, pcm);
            /* empty asm, emits nothing: z depends on pcm and feeds the Rot and the Add, so in sched2 it takes the
               second issue slot next to the lw and the mat.r[1] lqc2 waits a cycle (target order lw, lqc2, por, Rot) */
            int z; __asm__("" : "=r"(z) : "r"(pcm));
            qword d = Sub(AddZ(RotKZ(mat, v, vf.q, z), pos, z), pcm->pos);
            __asm__("" : "+j"(d)); /* empty asm: extra refs so d is caller-saved in vf1 */
            if (!FInsideCmFrustum(&D_002DDF40, d, vf))
                goto next;
            float g;
            if (!FInsideCmMrd(&D_002DDF40, d, plo->g38, &g))
                goto next;
            VECTOR_opAssign(&pl->x30, pos);
            cLight++;
            pl->r[0] = mat.r[0];
            pl->r[1] = mat.r[1];
            pl->r[2] = mat.r[2];
            pl->w30 = 1.0f;
            ((int *)&pl->r[2])[3] = 0;
            ((int *)&pl->r[1])[3] = 0;
            ((int *)&pl->r[0])[3] = 0;
            pl++;
            }
        next:
            plo = plo->x128;
            if (plo) goto top;
    done:;
    } else {
        register CM *pcm asm("$4") = &D_002DDF40; /* register pin: the target builds &D_002DDF40 in a0 */
        qword v = p->vF0;
        __asm__("" : "+j"(v)); /* empty asm: keeps vF0 in a pseudo */
        VU_FLOAT vf(p->g100);
        if (!FInsideCmFrustum(pcm, SubE(v, pcm->pos), vf))
            goto none;
        cLight = 1;
        aLight = &D_002A2D30;
    }
    if (cLight == 0) {
    none:
        STACKSCOPE_ctor_1AE4A0(&frame, 2);
        return;
    }
    int c = 0;
    DMA *pdma = PrplAllocRpl();
    pdma->x0 = 0xC;
    pdma->x4 = 0;
    int save = D_002DEA28.xC;
    AddDmaCnt__4DMAS(&D_002DEA28);
    for (Q *pq = p->pqFirst; pq; pq = pq->pqNext) {
        BUF *pb = &pq->abuf[pq->i];
        for (int i = 0; i < cLight; i++) {
            if (c < 2) {
                func_0013A688(p);
                c++;
            }
            unsigned short as[3];
            as[0] = 0x34;
            int ten = 10;
            as[1] = pq->c * ten + 0x34;
            as[2] = pq->c;
            AddVifUnpack__4VIFS3UPKiPvi(&D_002DEA28, 9, 1, as, 0, ten);
            AddVifUnpack__4VIFS3UPKiPvi(&D_002DEA28, 0xC, 4, &aLight[i], 7);
            AddVifStcycl__4VIFSii(&D_002DEA28, 1, 10);
            if (p->x43C == 1)
                AddVifUnpackRefs__4VIFS3UPKiPviPPPv(&D_002DEA28, 0xC, pq->c, pb->b, as[0] + 1, 0);
            AddVifUnpackRefs__4VIFS3UPKiPviPPPv(&D_002DEA28, 0xC, pq->c, pb->c, as[0] + 2, 0);
            AddVifUnpackRefs__4VIFS3UPKiPviPPPv(&D_002DEA28, 0xC, pq->c, pb->a, as[0] + 3, 0);
            AddVifStcycl__4VIFSii(&D_002DEA28, 1, 1);
            switch (p->x43C) {
            case 0: AddVifMscal__4VIFSPv(&D_002DEA28, D_00000000); break;
            case 1: AddVifMscal__4VIFSPv(&D_002DEA28, D_00000390); break;
            case 2: AddVifMscal__4VIFSPv(&D_002DEA28, D_00000870); break;
            }
            AddVifFlush__4VIFS(&D_002DEA28);
        }
    }
    EndDmaCnt__4DMAS(&D_002DEA28);
    AddDmaRet__4DMAS(&D_002DEA28);
    pdma->x8 = save;
    STACKSCOPE_ctor_1AE4A0(&frame, 2);
}
