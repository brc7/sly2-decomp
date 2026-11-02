struct G { char pad[0x24]; float f24; char pad2[0x24]; float f4c; };
extern struct G D_002DDF40;
struct T { int a; float b; float c; };
/** @sly2-annot addr=0x14FCC8
 * Purpose: Default slot 2 Activate(psoFocus, pv): pvHero(+0)=psoFocus; save g_cm fovTarget (+4) and sNearClip
 *   (+8)
 * Info: proposed name ActivateCplcy (T1); class CPLCY; TU cplcy
 * Notes: names_v2 v1 proposed _GLOBAL_$I$StartupCm__Fv (0.000)
 * Confidence: high [T1 0.80] - evidence: semantic review; static naming pass; sources: r2_131F70
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#tu_12e600-0x12e600-0x12eb48,
 *   docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void func_0014FCC8(struct T *p, int a)
{
    p->a = a;
    p->c = D_002DDF40.f4c;
    p->b = D_002DDF40.f24;
}
