typedef int qword __attribute__((mode(TI)));
struct C { char pad[0x60]; void *p60; };
struct B { char pad[0xEC]; struct C *pc; };
struct A { int x0; struct B *pb; };
int func_00132B10(void *p, int n);
int func_001CEB08(void *p, struct A *pa, qword q0, qword q1);
void func_001CE980(void *p);
/** @sly2-annot addr=0x1CFBE0
 * Purpose: SHRAPNEL FBounce: FBounceRip then remove on condition
 * Info: proposed name FBounceShrapnel (T1); TU rip
 * Notes: names_v2 v1 proposed InitBurst__FP5BURSTP6VECTORfP2SO (0.173)
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
int func_001CFBE0(void *p, struct A *pa, qword q0, qword q1)
{
    void *plo = pa->pb->pc->p60;
    if (func_00132B10(plo, 0x32)) return 0;
    if (func_00132B10(plo, 0x59)) {
        func_001CE980(p);
        return 1;
    }
    return func_001CEB08(p, pa, q0, q1);
}
