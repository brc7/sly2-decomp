// MATCH (Claude agent, lane E21), from fresh3 (56). Fixes: (1) for-increment written `k++, i++` (loop.c prepends each biv to loop_iv_list, so the LAST-incremented biv's giv inits are emitted first in the preheader; the target computes i*16 before k*16/k*4, which clobbers v1 and stops reload inheriting k from the k<c test); (2) Dot's d output `=j` (not earlyclobber) so it shares a's vf2.
// DRAFT (Claude agent, lane E5): fresh rewrite, aligned 49 (old 80). Outer while(k<c && (pblk || (pblk=alloc))) gives the target loop shape; GRand(&p->lm) inline gives the hoisted plm slot; unsigned n0 &= ~1u gives lui/ori. Left: giv bases (target base = &apd[i] for a-side, separate &afr[1].apos[i] giv for the copy).
typedef int qword __attribute__((mode(TI)));
struct EMITTER;
struct PD { unsigned int n0; int n4; float g8; float gC; };
struct PAIR { qword a; qword b; };
struct FR { qword apos[43]; qword av[43]; qword apd[43]; qword pad[3]; };
struct BLK {
    FR afr[2]; int x1080;
    int c; PAIR *apair; int pad108C; BLK *pblkNext;
};
struct PRG;
struct PRGVT { char pad[0x198]; short d; short pad2; void (*pfn)(PRG *, EMITTER *); };
struct PRG {
    char pad0[0xC]; PRGVT *pvt; char pad10[0x160 - 0x10]; int k160; char pad164[0x1A0 - 0x164]; int c1A0;
    char pad1A4[0x3B0 - 0x1A4]; int c3B0; char pad3B4[0x434 - 0x3B4]; int f434; int pad438; int k43C; int pad440;
    int c444; BLK *pblkFirst;
};
struct LM { float gMin, gMax; };
struct EMITTER { char pad[0xC8]; int fC8; char padCC[0xF0 - 0xCC]; int kF0; int fF4; LM lm; };
struct EMITX { PRG **ppprg; void *ppripg; EMITTER *pemitter; };
struct CM { char pad[0x70]; qword dir; };
extern "C" {
extern CM D_002DDF40;
extern char D_003DD490[];
PRG *func_001390D8(void *pv);
void func_001394C8(PRG *pprg, EMITTER *p);
BLK *func_00138FF8(void *pv, PRG *pprg);
void *func_002333B4(void *pv, int b, int cb);
int NRandInRange__Fii(int nMin, int nMax);
float GRandInRange__Fff(float gMin, float gMax);
void func_0015B428(EMITTER *p, EMITX *px, int c, qword *apos, qword *av, float *ag1, float *ag2, qword *aa, qword *ab);
}

static inline float Dot(qword a, qword b)
{
    qword d, t;
    float g;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(t) : "j"(a), "j"(b));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
}

static inline float GRand(LM *plm) { return GRandInRange__Fff(plm->gMin, plm->gMax); }

void func_0015B428(EMITTER *p, EMITX *px, int c, qword *apos, qword *av, float *ag1, float *ag2, qword *aa, qword *ab)
{
    PRG *pprg = 0;
    if (px && px->ppprg)
        pprg = *px->ppprg;
    if (!pprg) {
        pprg = func_001390D8(D_003DD490);
        if (!pprg)
            return;
        if (px && px->ppprg)
            *px->ppprg = pprg;
        func_001394C8(pprg, p);
        if (px && px->pemitter)
            pprg->pvt->pfn((PRG *)((char *)pprg + pprg->pvt->d), px->pemitter);
        if (!pprg)
            return;
    }
    BLK *pblk = pprg->pblkFirst;
    int k = 0;
    while (k < c && (pblk || (pblk = func_00138FF8(D_003DD490, pprg)))) {
        qword *pq1 = pblk->afr[1].apos;
        for (unsigned int i = pblk->c; i < 43 && k < c; k++, i++) {
            FR *pfr = &pblk->afr[0];
            pfr->apos[i] = apos[k];
            ((float *)&pfr->apos[i])[3] = ag1[k];
            pfr->av[i] = av[k];
            ((float *)&pfr->av[i])[3] = ag2[k];
#define pd ((PD *)&pfr->apd[i])
            func_002333B4(pd, 0, sizeof(PD));
            pd->n0 = NRandInRange__Fii(0, pprg->c3B0 - 1);
            if (pprg->f434)
                pd->n0 &= ~1u;
            if (p->fC8)
                pd->n4 = NRandInRange__Fii(0, pprg->c1A0 - 1);
            if (pprg->k43C == 2) {
                float g = GRand(&p->lm);
                pd->g8 = g;
                if (!p->kF0 && 0.0f < Dot(D_002DDF40.dir, pfr->av[i]))
                    pd->g8 = -g;
                if (p->fF4)
                    pd->gC = GRandInRange__Fff(-3.14159265f, 3.14159265f);
            }
            if (pprg->k160 == 2) {
                PAIR *ppair = &pblk->apair[i];
                ppair->a = aa[k];
                ppair->b = ab[k];
            }
            pq1[i] = pfr->apos[i];
            pq1[i + 43] = pfr->av[i];
            pq1[i + 86] = *(qword *)pd;
            pblk->c++;
            pprg->c444++;
        }
        pblk = pblk->pblkNext;
    }
}
