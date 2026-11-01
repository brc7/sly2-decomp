struct P { int a; int b; };
struct E { char pad[0x94]; int f94; int f98; int f9c; char pad2[0x100 - 0xa0]; };
struct S { char pad[0x50]; int c; struct E *ae; };
void func_00131B10(void *, struct S *, struct E *);
/** @sly2-annot addr=0x131B60
 * Purpose: For each table entry matching any (a,b) filter pair, DetachAsegaChannel
 * Info: proposed name DetachAsegaChannels (T2); TU asega
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#asega-0x1308a8-0x131f70,
 *   docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_00131B60(void *pv, struct S *p, int cp, struct P *ap) {
    int i, j;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        for (j = 0; j < cp; j++) {
            struct P *pp = &ap[j];
            if ((pe->f9c == pp->a || pp->a == -1) && (pe->f94 == pp->b || pp->b == -1)) {
                func_00131B10(pv, p, pe);
                break;
            }
        }
    }
}
