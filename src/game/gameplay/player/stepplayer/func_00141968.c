typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } UQ;
struct VTE { short d; short pad; void (*pfn)(void *, qword); };
struct E8 { char pad[0x80]; qword q; };
struct P {
    char pad0[0xC]; struct VTE *vt; char pad1[0xE8 - 0x10]; struct E8 *pE8;
    char pad2[0x248 - 0xEC]; float dz; char pad3[0x2E8 - 0x24C]; int f2E8; int f2EC; char pad4[0x2F4 - 0x2F0];
    float g2F4; char pad5[0x300 - 0x2F8]; int k300; char pad6[0x360 - 0x304]; UQ v360;
    char pad7[0xDF4 - 0x370]; int xDF4;
};
qword func_001A62E8(struct P *p);
qword func_001D6848(qword a, qword b, float g, qword c);
void func_00195A00(void *p, int a, int b, int c);
/** @sly2-annot addr=0x141968
 * Purpose: Splice: compute ballistic velocity from pos(+dz) toward target (0x1D6848 with g,h), SetVelocity
 *   (vt35), and if not hurt put jtair into launched state 8 with apex params
 * Info: proposed name LaunchStepplayer (T2); TU stepplayer
 * Notes: names_v2 C proposed JumpJlo__FP3JLO (0.638)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00141968(struct P *p, qword v, float g, float h)
{
    UQ pos;
    UQ r;
    pos.q = func_001A62E8(p);
    pos.f[2] += p->dz;
    r.q = func_001D6848(pos.q, v, g, p->pE8->q);
    p->vt[35].pfn((char *)p + p->vt[35].d, r.q);
    if (p->xDF4 == -1) {
        func_00195A00((char *)p + 0x2C0, 8, 0, 0);
        p->k300 = 3;
        p->v360.q = r.q;
        p->v360.f[2] = 0.0f;
        p->g2F4 = h;
        if (h == p->v360.f[2]) {
            p->f2E8 = 0;
            p->f2EC = 0;
        }
    }
}
