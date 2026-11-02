typedef struct CRef { int m_tagk; void *m_p; } CRef;
void InitTag__4CRef(CRef *pref, int tagk);
void func_00129658(CRef *pref, CRef *prefSrc);
void PairPtr_Assign(void *p, void *q);
/** @sly2-annot addr=0x120C48
 * Purpose: Shared body of set-car!/set-cdr!: bifk 0x23 sets node->ref, else node->pnext; returns Void
 * Info: proposed name RefOpSetCadr (T1); TU bif
 * Notes: names_v2 B proposed RefOpCdr__FiP4CRefP6CFrame (0.866)
 * Confidence: high [T1 0.90] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *RefOpSetCadr__FiP4CRefP6CFrame4BIFK(CRef *ret, void *pbasic, CRef *aref, int cref, int k)
{
    char *p = aref[0].m_p;
    if (k == 0x23)
        func_00129658((CRef *)(p + 8), &aref[1]);
    else if (aref[1].m_tagk == 0x1F)
        PairPtr_Assign(p + 0x10, aref[1].m_p);
    else
        PairPtr_Assign(p + 0x10, 0);
    InitTag__4CRef(ret, 0x24);
    return ret;
}
