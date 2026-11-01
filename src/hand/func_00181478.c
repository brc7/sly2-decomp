typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x1C0]; int n1C0; int x1C4; void *p1C8; void *p1CC; };
extern void func_001F36E0(struct O *p);
extern qword func_001A62E8(void *p);
extern int func_0017F550(struct O *p);
extern void func_001815E8(struct O *p, int n);
extern void *D_002DFB80;
extern float D_002BE880;
static __inline__ float GDist(qword a, qword b)
{
    qword d, one;
    float g;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xyz %0, %1, %1" : "=j"(d) : "j"(d));
    __asm__("vaddw.x %0, $vf0, $vf0w\n\tvadday.x ACC, %1, %1y\n\tvmaddz.x %1, %0, %1z" : "=&j"(one), "+j"(d));
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
}
static __inline__ int FClose(float g)
{
    int f = 1;
    if (g < D_002BE880) {} else f = 0;
    return f;
}
/** @sly2-annot addr=0x181478
 * Purpose: vt16: UpdateSo; state 0 waits on the CID 0x48 singleton D_002DFB80, then state/anim progression
 * Info: proposed name UpdateHacklock (T2); class HACKLOCK slot16; TU tu_17F090
 * Notes: names_v2 v1 proposed BindSgg__FP3SGG (0.011) | Cid4B->Cid75 (CID 0x4B=75, hex-literal) | class
 *   identity from whole_game WE names/object strings: UpdateCid75 -> UpdateHacklock
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r4b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_00181478(struct O *p)
{
    int s;
    func_001F36E0(p);
    do {
        s = p->n1C0;
        switch (s) {
        case 0:
            if (D_002DFB80) {
                if (FClose(GDist(func_001A62E8(D_002DFB80), func_001A62E8(p->p1CC)))) {
                    p->p1C8 = D_002DFB80;
                    func_001815E8(p, 1);
                }
            }
            break;
        case 1:
            if (func_0017F550(p))
                func_001815E8(p, 2);
            break;
        }
    } while (p->n1C0 != s);
}
