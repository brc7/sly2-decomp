struct VECTOR { float x, y, z; } __attribute__((aligned(16)));
struct EDGE { unsigned short aipos[2]; };
struct LSG {
    VECTOR posA;
    VECTOR posB;
    VECTOR normalA;
    VECTOR normalB;
    float gA, gB;
    int pad48;
    int fA, fB;
    EDGE *pedge;
    int iend;
    LSG *plsgNext;
};
extern "C" void *memmove(void *dst, const void *src, int cb);

// CategorizeAlsg (0x1A8410).  Keys: one function-scope plsg shared by both loops (its base becomes unknown to
// alias analysis, so the VECTOR swap temp at 0(sp) is not forwarded and fA/fB are reloaded); plain block temps
// for the float/int swaps; pipos formed just before the second memmove.
/** @sly2-annot addr=0x1A8410
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/physics-collision.md#mark-0x1a7c18-0x1aa978
 * @end-sly2-annot */
void CategorizeAlsg(int clsg, LSG *alsg, int *pcpos, int *aipos, LSG **aplsg, LSG **pplsgBoth)
{
    LSG *plsgNone = 0;
    int cpos = 0;
    int ilsg;
    int ipos;
    LSG *plsg;

    *pplsgBoth = 0;
    for (ilsg = 0; ilsg < clsg; ilsg++) {
        plsg = &alsg[ilsg];
        if (plsg->fA && plsg->fB) {
            plsg->plsgNext = *pplsgBoth;
            *pplsgBoth = plsg;
        } else if (!plsg->fA && !plsg->fB) {
            plsg->plsgNext = plsgNone;
            plsgNone = plsg;
        } else {
            if (plsg->fA) {
                VECTOR vecT;
                vecT = plsg->posA; plsg->posA = plsg->posB; plsg->posB = vecT;
                vecT = plsg->normalA; plsg->normalA = plsg->normalB; plsg->normalB = vecT;
                {
                    float gT = plsg->gA; plsg->gA = plsg->gB; plsg->gB = gT;
                }
                {
                    int fT = plsg->fA; plsg->fA = plsg->fB; plsg->fB = fT;
                }
                plsg->iend = 1;
            }
            for (ipos = 0; ipos < cpos; ipos++) {
                if (plsg->pedge->aipos[plsg->iend] == aipos[ipos])
                    break;
            }
            if (ipos >= cpos) {
                aipos[ipos] = plsg->pedge->aipos[plsg->iend];
                cpos++;
            } else {
                plsg->plsgNext = aplsg[ipos];
            }
            aplsg[ipos] = plsg;
        }
    }

    for (ipos = 0; ipos < cpos; ipos++) {
        LSG **pplsg = &aplsg[ipos];
        int *pipos;
        if ((*pplsg)->plsgNext)
            continue;
        for (plsg = plsgNone; plsg; plsg = plsg->plsgNext) {
            if (plsg->pedge->aipos[0] == aipos[ipos] || plsg->pedge->aipos[1] == aipos[ipos])
                goto next;
        }
        (*pplsg)->plsgNext = *pplsgBoth;
        *pplsgBoth = *pplsg;
        memmove(pplsg, pplsg + 1, (cpos - ipos) * 4 - 4);
        pipos = &aipos[ipos];
        memmove(pipos, pipos + 1, (cpos - ipos) * 4 - 4);
        ipos--;
        cpos--;
    next:;
    }
    *pcpos = cpos;
}
