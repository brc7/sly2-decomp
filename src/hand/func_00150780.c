typedef int qword __attribute__((mode(TI)));
typedef union VU { qword q; float f[4]; } VU;
struct G { char pad[0x90]; qword q90; };
extern struct G D_002DDF40;
extern char D_003DD490[];
struct S { int pad[4]; void *p10; };
qword func_001A62E8(void *p);
void *func_001C4A48(void *p);
/** @sly2-annot addr=0x150780
 * Purpose: Center: ploOwner(+0x10) pos, else player (0x1C4A48) pos +150 z, else camera pos
 * Info: proposed name PosCpzoomCenter (T1); TU cplcy
 * Confidence: mid (draft) [T1 0.50] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/camera.md#cplcy-0x14fca8-0x1520c8
 * @end-sly2-annot */
qword func_00150780(struct S *p)
{
    VU v;
    if (p->p10) {
        v.q = func_001A62E8(p->p10);
    } else {
        void *o = func_001C4A48(D_003DD490);
        if (o) {
            v.q = func_001A62E8(o);
            v.f[2] += 150.0f;
        } else {
            v.q = D_002DDF40.q90;
        }
    }
    return v.q;
}
