typedef struct AREQ { int cb; int cbAlign; } AREQ;
typedef struct CRef { int m_tagk; union { int n; float g; void *pv; } u; } CRef;
typedef struct NODE { int pad[2]; CRef ref; struct NODE *pnext; } NODE;
typedef struct ARR { int c; short *a; } ARR;
short *func_001AE318(AREQ *preq);
/** @sly2-annot addr=0x1252D0
 * Purpose: Counts a list and allocates (PvAllocSlot-style 0x1AE318) a U16 array of its S32/Symid elements
 *   (save-var path)
 * Info: proposed name FillAsFromPair (T1); TU bif
 * Notes: names_v2 B proposed RefOpStartSound__FiP4CRefP6CFrame (0.977) | splice_scripts format doc cites this
 *   function by this name (CONFIRMED(code) in the doc)
 * Confidence: mid (draft) [T1 0.65] - evidence: static naming pass; sources:
 *   r1_116918+format-doc:splice_scripts
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
void FillAsFromPair(NODE *pnode, ARR *parr)
{
    NODE *p;
    int i;
    parr->c = 0;
    parr->a = 0;
    for (p = pnode; p; p = p->pnext)
        parr->c++;
    if (parr->c) {
        AREQ req;
        req.cb = parr->c * 2;
        req.cbAlign = 0x10;
        parr->a = func_001AE318(&req);
        for (p = pnode, i = 0; p; p = p->pnext, i++) {
            if (p->ref.m_tagk == 0x15 || p->ref.m_tagk == 0x18)
                { int n = p->ref.u.n; parr->a[i] = n; }
        }
    }
}
