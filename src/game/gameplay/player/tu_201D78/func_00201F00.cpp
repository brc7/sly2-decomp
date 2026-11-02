class OBase { public: int a, b, c; virtual void V1(); virtual void V2(); virtual void V3(); virtual void V4(); virtual void V5();
 virtual void V6(); virtual void V7(); virtual void V8(); };
struct O { char pad[0x28]; unsigned long fl; char p2[0x100 - 0x30]; OBase *p100; };
struct S { char pad[0x30]; int c; O *a[1]; };
/** @sly2-annot addr=0x201F00
 * Purpose: vt slot 8 on companion of each visible target
 * Info: proposed name NotifyReconTargets (T2); TU tu_201D78
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/player.md#tu_201d78-0x201d78-0x203948
 * @end-sly2-annot */
extern "C" void func_00201F00(S *p)
{
    int i;
    for (i = 0; i < p->c; i++) {
        O *o = p->a[i];
        if (((int)(o->fl >> 60) & 1) && o->p100) o->p100->V8();
    }
}
