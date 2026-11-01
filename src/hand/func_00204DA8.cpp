/* SetStepGravityRatio (Sly 1 name): scale the step's gravity to rGravity (at least 0.1) and its mass so the weight stays the same.
   Ported from Lilly's personal decomp (T0981). Each VU float is a one-qword struct so it gets the stack home the dead sqc2 go to;
   the gravity is read as a struct member, and the old ratio goes into vf3 straight from memory ("rV"). No register pins. */
typedef int qword __attribute__((mode(TI)));
struct VU_FLOAT { qword qw; };
struct VU_VECTOR4 { qword qw; };
struct SOPHYS { char pad[0x80]; struct VU_VECTOR4 dvGravity; char pad90[0x40]; float m; };
struct STEP { char pad[0xE8]; struct SOPHYS *psophys; char padEC[0x260 - 0xEC]; float rUnk260; };
extern "C" void func_001F35C8(struct STEP *pso, float m);   /* SetSoMass */

/** @sly2-annot addr=0x204DA8
 * Purpose: Clamp r>=0.1, SetSoMass(m*rOld/r), scale gravity vector by r/rOld, store rGravity +0x260
 * Info: proposed name SetStepGravityRatio (T1); TU step
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
extern "C" void func_00204DA8(struct STEP *pstep, float rGravity)
{
    float r = 0.1f;
    qword qwScaled;
    r = rGravity > r ? rGravity : r;
    func_001F35C8(pstep, pstep->psophys->m * pstep->rUnk260 / r);
    {
        struct VU_FLOAT vuR;
        qword qwGrav = pstep->psophys->dvGravity.qw;
        __asm__("qmtc2 %1, %0" : "=j"(vuR.qw) : "r"(r));
        __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qwScaled) : "j"(qwGrav), "j"(vuR.qw));
    }
    {
        struct VU_FLOAT vuOld;
        qword qwResult;
        __asm__("qmtc2 %1, %0" : "=j"(vuOld.qw) : "rV"(*(int *)&pstep->rUnk260));
        __asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q" : "=j"(qwResult) : "j"(qwScaled), "j"(vuOld.qw));
        pstep->psophys->dvGravity.qw = qwResult;
    }
    pstep->rUnk260 = r;
}
