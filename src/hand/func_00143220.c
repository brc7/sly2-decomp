typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } VU;
struct VTE { short d; short pad; qword (*pfn)(void *); };
struct O { int pad[3]; struct VTE *vt; };
struct S { int k; float g4; float g8; int pad; qword pos; qword dir; };
extern qword D_002A2DC0;
qword func_001A62E8(struct O *p);
/** @sly2-annot addr=0x143220
 * Purpose: vt96: velocity minus vertical part -> dir; pos = pos + dir*dt; radii 150/75
 * Info: proposed name GetStepplayerStunHold (T1); class STEPPLAYER slot96; TU stepplayer
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540,
 *   docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_00143220(struct O *p, struct S *ps, float g)
{
    struct VTE *e = &p->vt[33];
    qword v, h, pos, s, r;
    VU u;
    v = e->pfn((char *)p + e->d);
    {
        qword d, one, up = D_002A2DC0;
        __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(d), "=&j"(one) : "j"(v), "j"(up));
        __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(d) : "j"(up), "j"(d));
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(h) : "j"(v), "j"(d));
    }
    pos = func_001A62E8(p);
    __asm__("qmtc2.ni %1, %0" : "=j"(s) : "r"(g));
    __asm__("vadda.xyzw ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(pos), "j"(h), "j"(s));
    ps->dir = h;
    ps->g4 = 150.0f;
    ps->g8 = 75.0f;
    ps->pos = r;
    u.q = s;
    ps->k = 0;
}
