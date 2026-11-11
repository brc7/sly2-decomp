// dzgcore.cpp -- the DZG constraint solver (Sly 1's dzg): a set of DZ constraints in four lists (active, at lower
// bound, at upper bound, free), solved by repeated Crout decompositions.
// It is an active-set solver for bounded linear constraints (the physics' contact and joint impulses): constraints
// move between the lists as they hit or leave their bounds, and each change re-solves the active system.
// Entry points: InitDzg, ClearDzgSolution, SolveDzgFilled, SolveInequalities.
// TU 0x11A550..0x11AFE0 (7 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_dzgcore
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_dzgcore
#include "sly2/dzg.h"
#include "sly2/slotheap.h"

// 0x11AE88 StaticInit_dzgcore and GlobalConstructors_dzgcore: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* DZ and DZG are in sly2/dzg.h (DL/DLE from sly2/basic.h). The iterator stays TU-private (five words, its own global
   g_pdliFirst) so the inline DLI helpers below stay as the per-function file wrote them. */
struct DLI { DL *pdl; void **ppv; int ibDle; DLI *pdliNext; int nUnused; };

typedef struct FRAME { int an[4]; } FRAME;
extern DLI *g_pdliFirst __asm__("g_pdliFirst");
extern "C" {
extern const float D_002A4120;   /* defined below where the ELF has it (.rodata order) */
extern const float D_002A4124;   /* defined below where the ELF has it (.rodata order) */
void *AllocClearedFromStack(SWALLOC *pmsg);
void *AllocFromStack(SWALLOC *pmsg);
void *func_002333B4(void *pv, int b, unsigned int cb) __asm__("memset");     /* memset */
void STACKSCOPE_ctor(FRAME *pframe);
void STACKSCOPE_ctor_1AE4A0(FRAME *pframe, int n) __asm__("_$_10STACKSCOPE");
DZG *InitDzg(DZG *pdzg, int cdzMax);
void SolveDzgFilled(DZG *pdzg);
}
void InitDl(DL *pdl, int ibDle);
void ClearDl(DL *pdl);
void ClearDle(DLE *pdle);
void AppendDlEntry(DL *pdl, void *pv);
void RemoveDlEntry(DL *pdl, void *pv);
void CopyAb(void *pvDst, void *pvSrc, unsigned int cb);
void CroutDecomp(int n, float *a, float *lu);
void CroutSolve(int n, float *lu, float *b, float *x);
void EnforceDzgDz(DZG *pdzg, DZ *pdz);

// Sets up a solver for at most maxConstraints constraints: allocates the DZ array and the matrices from the
// current allocator and initialises the four constraint lists. Same body as Sly 1's InitDzg.
// Also known as: InitDzgRows (upstream readable / Sly 1 name; InitDzgRows__FP3DZGi).
extern "C" DZG *InitDzg(DZG *dzg, int maxConstraints)
{
    SWALLOC msg;

    dzg->maxRows = maxConstraints;
    dzg->numActiveRows = 0;
    dzg->numRows = 0;
    msg.cb = maxConstraints * 0x28;
    msg.cbAlign = 0x10;
    dzg->rows = (DZ *)AllocClearedFromStack(&msg);
    func_002333B4(&dzg->activeRows, 0, sizeof(DL));
    func_002333B4(&dzg->rowsAtLowerBound, 0, sizeof(DL));
    func_002333B4(&dzg->rowsAtUpperBound, 0, sizeof(DL));
    func_002333B4(&dzg->freeRows, 0, sizeof(DL));
    InitDl(&dzg->activeRows, 0x20);
    InitDl(&dzg->rowsAtLowerBound, 0x20);
    InitDl(&dzg->rowsAtUpperBound, 0x20);
    InitDl(&dzg->freeRows, 0x20);
    msg.cb = maxConstraints * maxConstraints * 4;
    msg.cbAlign = 0x10;
    dzg->agMat = (float *)AllocFromStack(&msg);
    msg.cb = maxConstraints * maxConstraints * 4;
    msg.cbAlign = 0x10;
    dzg->agLu = (float *)AllocFromStack(&msg);
    msg.cb = maxConstraints * 4;
    msg.cbAlign = 0x10;
    dzg->rightHandSide = (float *)AllocFromStack(&msg);
    msg.cb = maxConstraints * 4;
    msg.cbAlign = 0x10;
    dzg->agX = (float *)AllocFromStack(&msg);
    return dzg;
}

// Clears the previous solution (every constraint's value and state) so the solver can be refilled.
void ClearDzgSolution(DZG *dzg)
{
    int i;

    dzg->numActiveRows = 0;
    ClearDl(&dzg->activeRows);
    ClearDl(&dzg->rowsAtLowerBound);
    ClearDl(&dzg->rowsAtUpperBound);
    ClearDl(&dzg->freeRows);

    for (i = 0; i < dzg->numRows; i++) {
        DZ *pdzs = &dzg->rows[i];
        pdzs->solution = pdzs->residual = 0.0f;
        ClearDle(&pdzs->dle);
        AppendDlEntry(&dzg->freeRows, pdzs);
    }
}

// EnforceDzgDz: Sly 1's dzg solver step (A/B/C/D constraint lists, Crout decomposition).
// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A4120 __asm__("D_002A4120") = 3.40282347e+38f; }
namespace RO_ORDER { extern const float r_D_002A4124 __asm__("D_002A4124") = -3.40282347e+38f; }
// Makes constraint dz hold (Sly 1's EnforceDzgDz): adds it to the active set, re-decomposes the active system and
// moves constraints that would violate their bounds to the bound lists, repeating until consistent.
void EnforceDzgDz(DZG *dzg, DZ *dz)
{
    int iter;
    if (dz) {
        RemoveDlEntry(&dzg->freeRows, dz);
        AppendDlEntry(&dzg->activeRows, dz);
        dzg->numActiveRows++;
    }
    for (iter = 0; iter < 10; iter++) {
        DZ *pdzI, *pdzJ, *pdzK, *pdzBest;
        float *pg;
        int i, k;
        float gMin;

        float *pgMat = dzg->agMat;
        for (pdzI = (DZ *)dzg->activeRows.head; pdzI; pdzI = (DZ *)pdzI->dle.next)
            for (pdzJ = (DZ *)dzg->activeRows.head; pdzJ; pdzJ = (DZ *)pdzJ->dle.next)
                *pgMat++ = pdzI->ag[pdzJ - dzg->rows];
        CroutDecomp(dzg->numActiveRows, dzg->agMat, dzg->agLu);
        float *pgB = dzg->rightHandSide;
        for (pdzI = (DZ *)dzg->activeRows.head; pdzI; pdzI = (DZ *)pdzI->dle.next)
            *pgB++ = -pdzI->residual;
        CroutSolve(dzg->numActiveRows, dzg->agLu, dzg->rightHandSide, dzg->agX);
        for (i = 0; i < dzg->numRows; i++) {
            float g = 0.0f;
            float *pgX = dzg->agX;
            for (pdzI = (DZ *)dzg->activeRows.head; pdzI; pdzI = (DZ *)pdzI->dle.next)
                g += *pgX++ * pdzI->ag[i];
            dzg->rows[i].residualSlope = g;
        }

        pdzBest = 0;
        gMin = 1.0f;
        k = 0;
        pg = dzg->agX;
        for (pdzI = (DZ *)dzg->activeRows.head; pdzI; pdzI = (DZ *)pdzI->dle.next) {
            float x = *pg++;
            if (x > 1e-4f) {
                // Empty asm, allocation-only operands (emits nothing): two extra uses of the +-1e-4 constants
                // raise their global-alloc priority above the hoisted D_002A4120/D_002A4124 loads, whose
                // REG_EQUIV-less pseudos otherwise win (local-alloc doubles the constants' live length), so
                // the constants get f5/f4 and the globals f7/f6 as in the target.
                __asm__("" : : "f"(1e-4f), "f"(-1e-4f));
                if (pdzI->maxValue != D_002A4120) {
                    float t = (pdzI->maxValue - pdzI->solution) / x;
                    if (t < gMin) {
                        gMin = t;
                        pdzBest = pdzI;
                        k = 4;
                    }
                }
            } else if (x < -1e-4f) {
                if (pdzI->minValue != D_002A4124) {
                    float t = (pdzI->minValue - pdzI->solution) / x;
                    if (t < gMin) {
                        gMin = t;
                        pdzBest = pdzI;
                        k = 2;
                    }
                }
            }
        }
        for (pdzK = (DZ *)dzg->rowsAtLowerBound.head; pdzK; pdzK = (DZ *)pdzK->dle.next) {
            if (pdzK->residualSlope > -1e-4f) {} else {
                float t = -pdzK->residual / pdzK->residualSlope;
                if (t < gMin) {
                    gMin = t;
                    pdzBest = pdzK;
                    k = 1;
                }
            }
        }
        for (pdzK = (DZ *)dzg->rowsAtUpperBound.head; pdzK; pdzK = (DZ *)pdzK->dle.next) {
            if (pdzK->residualSlope < 1e-4f) {} else {
                float t = -pdzK->residual / pdzK->residualSlope;
                if (t < gMin) {
                    gMin = t;
                    pdzBest = pdzK;
                    k = 3;
                }
            }
        }
        for (i = 0; i < dzg->numRows; i++) {
            DZ *pdzT = &dzg->rows[i];
            pdzT->residual += gMin * pdzT->residualSlope;
        }
        pg = dzg->agX;
        for (pdzI = (DZ *)dzg->activeRows.head; pdzI; pdzI = (DZ *)pdzI->dle.next)
            pdzI->solution += gMin * *pg++;

        switch (k) {
        case 0:
            return;
        case 2:
            dzg->numActiveRows--;
            RemoveDlEntry(&dzg->activeRows, pdzBest);
            AppendDlEntry(&dzg->rowsAtLowerBound, pdzBest);
            if (pdzBest == dz)
                return;
            break;
        case 1:
            dzg->numActiveRows++;
            RemoveDlEntry(&dzg->rowsAtLowerBound, pdzBest);
            AppendDlEntry(&dzg->activeRows, pdzBest);
            break;
        case 4:
            dzg->numActiveRows--;
            RemoveDlEntry(&dzg->activeRows, pdzBest);
            AppendDlEntry(&dzg->rowsAtUpperBound, pdzBest);
            if (pdzBest == dz)
                return;
            break;
        case 3:
            dzg->numActiveRows++;
            RemoveDlEntry(&dzg->rowsAtUpperBound, pdzBest);
            AppendDlEntry(&dzg->activeRows, pdzBest);
            break;
        }
    }
}

// Start iterating display list pdl with iterator pdli and push it on the global iterator stack (g_pdliFirst).
static inline void InitDli(DLI *pdli, DL *pdl)
{
    pdli->pdl = pdl;
    pdli->ppv = (void **)pdl;
    pdli->ibDle = pdl->linkOffset;
    pdli->pdliNext = g_pdliFirst;
    g_pdliFirst = pdli;
}

// Return the current element of the iterator and advance it along the element's link field.
static inline void *NextDliEntry(DLI *pdli)
{
    void *pv = *pdli->ppv;
    pdli->ppv = (void **)((char *)pv + pdli->ibDle);
    return pv;
}

// Solves a filled DZG: moves equality constraints into the active list, enforces them, then repeatedly enforces
// the most violated remaining constraint until none is violated. Sly 1's SolveDzg loop without its lazy FillDzgDz.
// Also known as: EnforceDzgConstraints (upstream readable / Sly 1 name; EnforceDzgConstraints__FP3DZG).
extern "C" void SolveDzgFilled(DZG *dzg)
{
    DLI dli;
    DZ *pdz;

    InitDli(&dli, &dzg->freeRows);
    while ((pdz = (DZ *)NextDliEntry(&dli)) != 0) {
        if (pdz->kind == 1) {
            RemoveDlEntry(&dzg->freeRows, pdz);
            AppendDlEntry(&dzg->activeRows, pdz);
            dzg->numActiveRows++;
        }
    }
    EnforceDzgDz(dzg, 0);
    g_pdliFirst = dli.pdliNext;

    for (;;) {
        DZ *pdzMin = 0;
        float gMin = -0.01f;
        DZ *pdzT;
        for (pdzT = (DZ *)dzg->freeRows.head; pdzT; pdzT = (DZ *)pdzT->dle.next) {
            if (pdzT->residual < gMin) {
                gMin = pdzT->residual;
                pdzMin = pdzT;
            }
        }
        if (!pdzMin) {
            break;
        }
        EnforceDzgDz(dzg, pdzMin);
    }
}

// Fill the message and allocate cb bytes (kind nKind) from the frame stack.
static inline void *AllocStackBlock(SWALLOC *pmsg, int cb, int nKind) { pmsg->cb = cb; pmsg->cbAlign = nKind; return AllocFromStack(pmsg); }

// Solves `count` inequality constraints with a temporary DZG on the stack: row i of the count x count matrix A
// and rhs[i] define constraint i, each bounded above by maxValue; writes the solved values to outX.
// Also known as: SolveDzgConstraints (upstream readable / Sly 1 name; SolveDzgConstraints__FiPfN21f).
extern "C" void SolveInequalities(int count, float *matrix, float *rhs, float *outX, float maxValue)
{
    FRAME frame;
    DZG solver;
    SWALLOC msg;
    int i;

    STACKSCOPE_ctor(&frame);
    InitDzg(&solver, count);
    for (i = 0; i < count; i++) {
        DZ *dz = &solver.rows[i];
        dz->maxValue = maxValue;
        dz->ag = (float *)AllocStackBlock(&msg, count * sizeof(float), 0x10);
        CopyAb(dz->ag, matrix + i * count, count * sizeof(float));
        dz->residual = rhs[i];
        AppendDlEntry(&solver.freeRows, dz);
    }
    solver.numRows = count;
    SolveDzgFilled(&solver);
    for (i = 0; i < count; i++) {
        outX[i] = solver.rows[i].solution;
    }
    STACKSCOPE_ctor_1AE4A0(&frame, 2);
}
