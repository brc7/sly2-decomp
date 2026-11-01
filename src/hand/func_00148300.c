/* func_00148300 (Claude agent): case 2 before case 0, and a block-local r per case (so the g38 load ties to f20). */
typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(void *); };
struct SO { char pad[0xC]; struct VTE *vt; char pad1[0x14 - 0x10]; int id; char pad2[0x28 - 0x18]; long x28; char pad3[8]; float g38; };
struct G { char pad[0x14]; int grf; char pad2[0x90 - 0x18]; qword pos; };
struct P { char pad[0x2C]; char m2C[4]; };
extern int D_003E0E70;
extern struct G D_002DDF40;
void *func_001C4840(void);
qword func_001A62E8(void *p);
int FFindDlEntry__FP2DLPv(void *p, int id);
void AppendDlEntry__FP2DLPv(void *p, int id);
void RemoveDlEntry__FP2DLPv(void *p, int id);
static __inline__ float GDistSq(qword a, qword b)
{
    qword d, t;
    float g;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xyz %0, %0, %0\n"
        "vaddw.x %1, $vf0, $vf0w\n"
        "vadday.x ACC, %0, %0y\n"
        "vmaddz.x %0, %1, %0z" : "+j"(d), "=j"(t));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
}
void func_00148300(struct P *p, struct SO *pso)
{
    int grf;
    qword posA, posB;
    void *po;
    int fIn, id;
    if (D_003E0E70) return;
    posA = D_002DDF40.pos;
    grf = D_002DDF40.grf;
    po = func_001C4840();
    if (po) posB = func_001A62E8(po);
    else posB = posA;
    id = pso->id;
    switch ((int)((pso->x28 << 8) >> 60)) {
    case 1:
        fIn = 1;
        break;
    case 2:
        fIn = 0;
        break;
    case 0:
        if (grf & ((unsigned char *)pso)[0x2D]) {
            float r;
            fIn = 1;
            r = pso->g38 * pso->g38;
            fIn = GDistSq(func_001A62E8(pso), posA) < r;
        } else {
            fIn = 0;
        }
        break;
    case 3:
        if (grf & ((unsigned char *)pso)[0x2D]) {
            float r;
            fIn = 1;
            r = pso->g38 * pso->g38;
            fIn = GDistSq(func_001A62E8(pso), posB) < r;
        } else {
            fIn = 0;
        }
        break;
    }
    if (fIn == FFindDlEntry__FP2DLPv(p->m2C, id)) return;
    if (fIn) {
        AppendDlEntry__FP2DLPv(p->m2C, id);
        pso->vt[23].pfn((char *)pso + pso->vt[23].d);
    } else {
        pso->vt[22].pfn((char *)pso + pso->vt[22].d);
        RemoveDlEntry__FP2DLPv(p->m2C, id);
    }
}
