typedef int qword __attribute__((mode(TI)));
struct HIT { qword v0; char pad[0x28 - 0x10]; float g28; char pad1[0x40 - 0x2C]; float g40; char pad2[0x60 - 0x44]; };
struct CLS { CLS(); ~CLS(); char pad[0x10]; };
struct M { char pad[0x4]; int x4; char pad1[0x10 - 0x8]; qword v10; char pad2[0x30 - 0x20]; qword v30; qword v40; qword v50;
    char pad3[0x08]; float g48; };
struct O { char pad[0xEC]; int xEC; char pad1[0x208 - 0xF0]; int x208; };
extern "C" {
qword func_001A6F00(void *p, void *q, qword v);
void func_0020FAA0(void *pv, qword vMin, qword vMax, void *pfn, int n, int *pn0, int *pn1);
int func_0014CC58(int n, qword a, qword b, int n0, int n1, HIT *phit);
void func_0012DA30(void *p, qword v);
void func_0020F948();
extern char D_003DD490[];
extern qword D_002A2DC0;
extern qword D_002A2DD0;
int func_00189EC8(O *p, M *pm, void *px);
}
static inline qword VMin(qword a, qword b) { qword r; __asm__("vmini.xyz %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword VMax(qword a, qword b) { qword r; __asm__("vmax.xyz %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword VSub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
/** @sly2-annot addr=0x189EC8
 * Purpose: Probe the ground along the XMG lines (PsoHitTestLineObjects, normal.z>0.7) and set foot target
 * Info: proposed name FMatchJtXmg (T1); TU jt
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
int func_00189EC8(O *p, M *pm, void *px)
{
    if (pm->x4) {
        HIT hit;
        CLS cls;
        union { qword q; } u;
        int n0, n1;
        qword a, b, c;
        a = func_001A6F00(p, 0, pm->v30);
        b = func_001A6F00(p, 0, pm->v40);
        c = func_001A6F00(p, 0, pm->v50);
        func_0020FAA0(D_003DD490, VMin(a, VMin(b, c)), VMax(a, VMax(b, c)), (void *)func_0020F948, p->xEC, &n0, &n1);
        if ((func_0014CC58(0, a, b, n0, n1, &hit) || func_0014CC58(0, b, c, n0, n1, &hit)) &&
            hit.g40 != 0.0f && 0.7f < hit.g28) {
            qword w;
            hit.v0 = VSub(hit.v0, D_002A2DC0);
            w = func_001A6F00(0, p, hit.v0);
            u.q = w;
            func_0012DA30(px, VSub(w, pm->v10));
            return 1;
        } else {
            func_0012DA30(px, D_002A2DD0);
            if (p->x208 && ((float *)&pm->v40)[2] < -85.0f)
                return 0;
            return 1;
        }
    }
    return 1;
}
