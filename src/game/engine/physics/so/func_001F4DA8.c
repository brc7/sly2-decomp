struct N { int pad[2]; void *p8; char pad2[0xAC - 0xC]; struct N *next; };
struct G;
extern struct G D_003DD490;
void func_001F5078(void *);
void FreeSwXpList__FP2SWP2XP(struct G *, struct N *);
/** @sly2-annot addr=0x1F4DA8
 * Purpose: Unlink contact nodes between pa and pb from list, free them (FreeSwXpList) and invalidate pa
 * Info: proposed name RemoveSoContactsWith (T2); TU so
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F4DA8(void *pa, void *pb, struct N **ppn)
{
    struct N *pfree = 0;
    struct N *pn;
loop:
    pn = *ppn;
    if (pn) {
        int i = pn->p8 == pa;
        if (((struct N *)((char *)pn + (i << 6)))->p8 == pb) {
            *ppn = pn->next;
            pn->next = pfree;
            pfree = pn;
        } else {
            ppn = &pn->next;
        }
        goto loop;
    }
    if (pfree) {
        func_001F5078(pa);
        FreeSwXpList__FP2SWP2XP(&D_003DD490, pfree);
    }
}
