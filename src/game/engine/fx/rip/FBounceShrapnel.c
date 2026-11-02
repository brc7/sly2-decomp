typedef int qword __attribute__((mode(TI)));
struct C { char pad[0x60]; void *p60; };
struct B { char pad[0xEC]; struct C *pc; };
struct A { int x0; struct B *pb; };
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int n);
int FBounceRip(void *p, struct A *pa, qword q0, qword q1);
void RemoveRip__FP3RIP(void *p);
/** @sly2-annot addr=0x1CFBE0
 * Purpose: SHRAPNEL FBounce: FBounceRip then remove on condition
 * Info: proposed name FBounceShrapnel (T1); TU rip
 * Notes: names_v2 v1 proposed InitBurst__FP5BURSTP6VECTORfP2SO (0.173)
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
int FBounceShrapnel(void *p, struct A *pa, qword q0, qword q1)
{
    void *plo = pa->pb->pc->p60;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(plo, 0x32)) return 0;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(plo, 0x59)) {
        RemoveRip__FP3RIP(p);
        return 1;
    }
    return FBounceRip(p, pa, q0, q1);
}
