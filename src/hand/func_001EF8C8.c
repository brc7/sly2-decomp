typedef int qword __attribute__((mode(TI)));
union V { qword q; float f[4]; };
struct VTE { short d; short pad; void (*pf)(void *, qword); };
struct S { char pad[0xC]; struct VTE *vt; char pad2[0x150-0x10]; int n150; };
struct G { char pad[0x30]; qword q; };
extern struct G D_002DDFA0;
void func_001F36E0(struct S *p);
qword func_001A62E8(struct S *p);
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0, $0, %1" : "=r"(z) : "r"(q)); return z; }
/** @sly2-annot addr=0x1EF8C8
 * Purpose: SKY vt16: UpdateSo, then TranslateToPos(camera pos D_002DDFA0, keeping own Z unless fFollowZ
 *   +0x150)
 * Info: proposed name UpdateSky (T1); class SKY slot16; TU sky
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/rendering.md#13-key-functions, docs/reference/functions/rendering.md#sky-0x1ef850-0x1efa90
 * @end-sly2-annot */
void func_001EF8C8(struct S *p)
{
    union V v;
    func_001F36E0(p);
    v.q = D_002DDFA0.q;
    if (!p->n150) v.f[2] = GetZ(func_001A62E8(p));
    p->vt[29].pf((char *)p + p->vt[29].d, v.q);
}
