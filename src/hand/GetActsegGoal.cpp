class C {
public:
    virtual void V1();
    virtual void V2();
    virtual void V3(int a, int b, float g0, float g1);
};
struct O { char pad[0x1c]; float f1c; char pad2[0x40 - 0x20]; float f40; };
struct S { int pad[7]; C *pc; int f20; O *po; };
extern "C" void FWrapAsegaTime(O *, float *, float *);
extern "C" void func_0012CF30(S *, int, int);
/** @sly2-annot addr=0x131D68
 * Purpose: Per-frame channel time wrap (0x1317D8) and virtual update
 * Info: proposed name GetActsegGoal (T1); TU asega
 * Notes: verifier WRONG: AsegaChnUpdate -> GetActsegGoal | verifier: purpose partly wrong: ACT-family vtable
 *   0x2CAF08 slot 5. The base slot 5 0x12CF30(p, pvalue, pvel) writes the current goal value (0x22CA18:
 *   position row or euler, by channel kind) an
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/animation.md#7-key-functions,
 *   docs/reference/functions/animation.md#asega-0x1308a8-0x131f70
 * @end-sly2-annot */
extern "C" void GetActsegGoal(S *p, int a, int b, float g) {
    if (p->pc) {
        O *po = p->po;
        float t, dt;
        dt = po->f40;
        t = po->f1c + g * dt;
        FWrapAsegaTime(po, &t, &dt);
        p->pc->V3(a, b, t, dt);
    } else {
        func_0012CF30(p, a, b);
    }
}
