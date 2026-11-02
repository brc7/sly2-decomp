struct G { char pad[0x24]; float g24; };
extern struct G D_002DDF40;
struct B { char pad[0x1BC]; float g; };
void func_0014FCC8(void *a, struct B *b);
void func_0014F610(struct G *pg, float g);
/** @sly2-annot addr=0x151900
 * Purpose: CPLCY::Activate; near clip 49; fovTarget = turret +0x1BC
 * Info: proposed name ActivateCpturret (T1); TU cplcy
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
void func_00151900(void *a, struct B *b)
{
    struct G *pg;
    func_0014FCC8(a, b);
    pg = &D_002DDF40;
    func_0014F610(pg, 49.0f);
    pg->g24 = b->g;
}
