typedef int qword __attribute__((mode(TI)));
struct P { char pad[0x2C0]; qword pos; qword vel; qword pos2; qword vel2; };
struct O { char pad[0x4F4]; int x4F4; };
struct S { qword a; qword b; qword c[2]; qword d; };
extern qword D_002A2DD0;
qword func_0014E698(struct P *p, struct S *ps);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct O *po, int k);
void func_0014E6D0(struct P *p, qword *ppos, qword *pvel, qword target, float k1, float k2);
/** @sly2-annot addr=0x14E768
 * Purpose: Springs dposAdjust/dposFocus (+0x2C0/+0x2E0) toward CalcCmAdjust; per STEPPLAYER mode +0x4F4
 *   (0/1/2) different gains; writes cpdefi.posAdjust = posBase + dposAdjust
 * Info: proposed name AdjustCmCpdefi (T1); TU cm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void func_0014E768(struct P *p, struct O *po, struct S *ps)
{
    qword q = func_0014E698(p, ps);
    if (FIsBasicDerivedFrom__FP5BASIC3CID(po, 0x34)) {
        switch (po->x4F4) {
        case 0:
            func_0014E6D0(p, &p->pos, &p->vel, q, 5.0f, -3.5f);
            func_0014E6D0(p, &p->pos2, &p->vel2, D_002A2DD0, 5.0f, -3.5f);
            break;
        case 1:
            func_0014E6D0(p, &p->pos, &p->vel, q, 1.0f, -10.0f);
            func_0014E6D0(p, &p->pos2, &p->vel2, p->pos2, 0.0f, -10.0f);
            break;
        case 2:
            func_0014E6D0(p, &p->pos, &p->vel, q, 5.0f, -3.5f);
            break;
        }
    } else {
        func_0014E6D0(p, &p->pos, &p->vel, q, 5.0f, -3.5f);
        func_0014E6D0(p, &p->pos2, &p->vel2, D_002A2DD0, 5.0f, -3.5f);
    }
    {
        qword r;
        __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(ps->b), "j"(p->pos));
        ps->d = r;
    }
}
