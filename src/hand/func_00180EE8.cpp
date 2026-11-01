typedef int qword __attribute__((mode(TI)));
typedef struct { qword a[3]; } MATRIX3;
struct VT { struct { short d; short pad; qword (*pfn)(void *); } a[40]; };
struct X { char pad[0xA0]; void *pA0; };
struct O { char pad[0xC]; struct VT *pvt; char pad1[0x5C - 0x10]; struct X *p5C; char pad2[0x1C0 - 0x60]; int n1C0;
    char pad3[0x204 - 0x1C4]; float g204; char pad4[0x210 - 0x208]; qword v210; char pad5[0x378 - 0x220]; char a378[0xC]; float g384; };
extern qword D_002A2DC0;
extern "C" MATRIX3 func_0011B500(qword v, float g);
extern "C" MATRIX3 func_0022FD38(void *pv, int n, MATRIX3 *pm);
extern "C" void func_001F59A0(struct O *p, MATRIX3 *pm, void *pv);
extern "C" void func_001FAC30(struct O *p, qword v);
static inline qword VSub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b) : "memory"); return r; }
static inline qword Bc(int g) { qword r; __asm__("qmtc2.ni %1, %0" : "=j"(r) : "r"(g)); return r; }
extern "C" void func_00180EE8(struct O *p);
/** @sly2-annot addr=0x180EE8
 * Purpose: vt70 override
 * Info: proposed name PresetHackenemyAccel (T2); class HACKENEMY slot70; TU tu_17F090
 * Notes: names_v2 C proposed RebindStepguardEnemy__FP9STEPGUARD (0.638) | CID49->CID73 (CID 0x49=73, vtable)
 *   | class identity from whole_game WE names/object strings: PresetCid73Accel -> PresetHackenemyAccel
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r4b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_00180EE8(struct O *p)
{
    if (p->n1C0 != 4) {
        MATRIX3 mat;
        mat = func_0011B500(D_002A2DC0, p->g204);
        if (p->n1C0 != 3)
            mat = func_0022FD38(p->p5C->pA0, 0, &mat);
        func_001F59A0(p, &mat, p->a378);
    }
    if (p->n1C0 == 3) {
        qword v = p->v210;
        qword w = p->pvt->a[33].pfn((char *)p + p->pvt->a[33].d);
        qword d, s, r;
        union { qword q; float f[4]; } u;
        d = VSub(v, w);
        {
        s = Bc(*(int *)&p->g384);
        }
        u.q = s;
        __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(d), "j"(s));
        func_001FAC30(p, r);
    }
}
