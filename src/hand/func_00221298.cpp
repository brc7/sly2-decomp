typedef int qword __attribute__((mode(TI)));
struct BSP;
struct O { char pad[0x2DC]; int x2DC; };
struct S { char pad[0xE0]; struct O *pE0; char pad1[0x108 - 0xE4]; struct BSP *p108; char pad2[0x124 - 0x10C]; int x124; char pad3[0x13C - 0x128]; int x13C; };
struct G;
extern struct G D_003DD490;
extern int D_002DE240;
extern "C" struct O *PpoCur(struct G *pg);
extern "C" void func_00221178(struct S *p, int k);
extern "C" qword func_001A62E8(struct O *po);
extern "C" qword ConvertAloPos(void *source, struct S *p, qword q);
extern "C" struct BSP *PbspPointInBspQuick(struct BSP *p, unsigned int limit, qword v);
extern "C" int FIsBasicDerivedFrom__FP5BASIC3CID(struct O *po, int n);
static inline int AutomaticSentinel(){int value;__asm__("addiu %0,$zero,-1":"=r"(value));return value;}
/** @sly2-annot addr=0x221298
 * Purpose: current player (0x1C4A48 PpoCur); retarget; inside test via tunnel BSP (0x118FB0) unless override;
 *   switch unless jumping
 * Info: proposed name UpdateTnCallback (T1); TU tn
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r8_21BB88
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
extern "C" void func_00221298(struct S *p)
{
    struct O *po = PpoCur(&D_003DD490);
    int f;
    if (!po) return;
    if (po != p->pE0) {
        func_00221178(p, 0);
        p->pE0 = po;
    }
    int initial=p->x13C;
    int sentinel=AutomaticSentinel();
    if(initial==sentinel) f=PbspPointInBspQuick(p->p108, 0x7FFFFFFF, ConvertAloPos(0, p, func_001A62E8(po))) != 0;
    else f=initial;
    if (p->x124) goto set;
    if (!FIsBasicDerivedFrom__FP5BASIC3CID(po, 0x34)) goto set;
    if (po->x2DC == -1) goto set;
    if (D_002DE240) goto set;
    if (p->x13C == -1) return;
set:
    func_00221178(p, f);
}
