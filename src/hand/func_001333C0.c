struct E { int id; int cref; struct E *pnext; };
struct S { int pad[2]; struct E *pfirst; };
extern char D_003E1144[];
struct E *func_0011CCC0(void *);
/** @sly2-annot addr=0x1333C0
 * Purpose: Add ploTarget to the object's subscriber list (BASIC+0x8), refcounted
 * Info: proposed name SubscribeLoObject (T1); TU basic
 * Notes: names_v2 C proposed PripgNew__FP2SW5RIPGT (0.438)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r2b_rest+verdicts_followups
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#basic-0x132b00-0x1336f0,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_001333C0(struct S *p, int id) {
    struct E *pe;
    for (pe = p->pfirst; pe; pe = pe->pnext) {
        if (pe->id == id) { pe->cref++; return; }
    }
    {
        register struct E *peNew __asm__("$3") = func_0011CCC0(D_003E1144);
        __asm__("" : "+r"(peNew));
        peNew->id = id;
        { register int one __asm__("$2") = 1; __asm__("" : "+r"(one)); peNew->cref = one; }
        peNew->pnext = p->pfirst;
        p->pfirst = peNew;
    }
}
