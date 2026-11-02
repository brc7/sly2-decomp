/* MATCH (Claude agent laneB9, 2026-10-23): v3 + second commented pin, pv in $5 (a1), pf = (i << 4) + pv. */
/* v3 (Claude agent laneB8, 2026-10-23): aligned 10 -> 2. ONE commented register pin: pf in $2 (v0), plus the tail written as `pf += j; f(*pf)` in all three arms (the sum then reuses pf like the target). Left: 1B-else `sll v0,a1,4; lw a1,pv` (target puts i<<4 in pf/v0, pv in a1; ours sll a1 in place, pv in v0). Tried: (i<<4)+char*, int sums, pf = base + i*4, &[i].x, j pin instead, i pin: 2-18. */
/* v2 (Claude agent, 2026-10-17): aligned 22 -> 10. Shared function-scope i / j / float *pf across cases (pidx = pidx->pnext reused) gives the target t0/a1/a3 registers and the cross-jumped pf[j] tail. Left: pf in v1, j in v0 (target pf v0, j v1) in the pf[j] tail. */
typedef struct VECTOR { float x, y, z, w; } VECTOR;
typedef struct CRef { int m_tagk; union { int n; float g; void *pv; } u; } CRef;
typedef struct IDX { int pad[3]; int i; struct IDX *pnext; } IDX;
void Decref__4CRef(CRef *pref);
CRef *func_001291A8(CRef *pref, CRef *prefSrc);
CRef *SetF32__4CReff(CRef *pref, float g);
CRef *CRef_SetVector(CRef *pref, VECTOR *pv);
/** @sly2-annot addr=0x1222C8
 * Purpose: Splice builtin "get-element": element of vector/matrix by index list
 * Info: proposed name RefOpGetElement (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *RefOpGetElement__FiP4CRefP6CFrame(CRef *ret, int carg, CRef *aref)
{
    CRef ref;
    IDX *pidx;
    int i;
    int j;
    register float *pf __asm__("$2"); /* register pin: pf must be v0 (j gets v1) in the shared pf[j] tail */
    ref.m_tagk = -1;
    pidx = (IDX *)aref[1].u.pv;
    switch (aref[0].m_tagk) {
    case 0x1B: {
        i = pidx->i;
        if (carg == 2) {
            CRef_SetVector(&ref, &((VECTOR *)aref[0].u.pv)[i]);
        } else {
            { register int pv __asm__("$5") = (int)aref[0].u.pv; /* register pin: pv must be a1 so i<<4 lands in pf (v0) */
              pf = (float *)((i << 4) + pv); }
            pidx = pidx->pnext;
            j = pidx->i;
            pf += j;
            SetF32__4CReff(&ref, *pf);
        }
        break;
    }
    case 0x1A: case 0x1C: case 0x1D:
        pf = (float *)aref[0].u.pv;
        j = pidx->i;
        pf += j;
            SetF32__4CReff(&ref, *pf);
        break;
    case 0x1E:
        pf = (float *)aref[0].u.pv;
        j = pidx->i;
        pf += j;
            SetF32__4CReff(&ref, *pf);
        break;
    }
    func_001291A8(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
