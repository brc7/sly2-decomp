struct E { int id; int cref; struct E *pnext; };
struct S { int pad[2]; struct E *pfirst; };
extern char D_003E1144[];
void func_0011CD98(void *, void *);
/** @sly2-annot addr=0x133448
 * Purpose: Decrement/remove ploTarget from the subscriber list
 * Info: proposed name UnsubscribeLoObject (T1); TU basic
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/object-model.md#basic-0x132b00-0x1336f0,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_00133448(struct S *p, int id) {
    struct E **ppe;
    struct E *pe;
    for (ppe = &p->pfirst; (pe = *ppe) != 0; ppe = &pe->pnext) { if (pe->id == id) { if (--pe->cref <= 0) { *ppe = pe->pnext; func_0011CD98(D_003E1144, pe); } break; } }
}
