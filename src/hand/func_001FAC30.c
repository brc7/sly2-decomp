typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x98]; short k; char pad2[0xA0 - 0x9A]; int nA0; char pad3[0xF0 - 0xA4]; qword pos; };
struct P { char pad[0x58]; struct O *po; };
int func_00230380(int);
qword func_0022FCC8(int, int, qword);
/** @sly2-annot addr=0x1FAC30
 * Purpose: If the position XF is in a velocity role (1-3): xf->dv(+0xF0) += world vector converted into the
 *   XF parent space
 * Info: proposed name AddSoAcceleration (T1); TU solve
 * Notes: names_v2 B proposed SetSaiIframe__FP3SAIi (0.856) | v3: superseded AddSoVelocity | top50_round
 *   (0.80): Sly 1 so.cpp: AddSoAcceleration(pso,dv){ pso->xf.dv += *dv; } and PresetSoAccel calls it. Sly 2
 *   body adds ConvertXfVec(0, xf->idParent(+0xA0), v) into +0xF0, the field SlotSetRole zeroes when entering
 *   a velocity role (dv
 * Confidence: high [T1 0.80] - evidence: Sly 1 / Lilly reference; semantic review; static naming pass;
 *   sources: r7_1F8DD8+top50_round
 * See: docs/engine/physics-collision.md#7-key-functions, docs/engine/player.md#3-step-stepcpp-0x203948,
 *   docs/reference/functions/physics-collision.md#solve-0x1f8dd8-0x1faf70
 * @end-sly2-annot */
void func_001FAC30(struct P *p, qword v)
{
    if (func_00230380(p->po->k)) {
        struct O *po = p->po;
        qword a = po->pos;
        qword r = func_0022FCC8(0, po->nA0, v);
        qword s;
        __asm__("vadd.xyzw %0, %1, %2" : "=j"(s) : "j"(a), "j"(r));
        po->pos = s;
    }
}
