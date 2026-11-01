typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[10]; };
struct E { char pad[0xDC]; void *pDC; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0xE8 - 0x10]; struct E *pe; char pad3[0x12C - 0xEC]; float g12C; float g130; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
void func_001A5E28(struct P *);
qword func_001A62E8(struct P *);
void *func_001C0D70(qword, int);
void func_001F37C8(struct P *, void *);
/** @sly2-annot addr=0x1F36E0
 * Purpose: SO vt16: UpdateLo; refresh region from position; advance fade alpha +0x12C by rate +0x130*dt,
 *   removing (vt9) at 0
 * Info: proposed name UpdateSo (T1); class SO slot16; TU so
 * Notes: names_v2 C proposed PxpFindSoGround__FP2SOT0Pi (0.200)
 * Confidence: mid (draft) [T1 0.70] - evidence: semantic review; static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/gameplay-objects.md#tu_223f70-0x223f70-0x2259d8,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F36E0(struct P *p)
{
    void *q;
    float d;
    func_001A5E28(p);
    q = p->pe ? p->pe->pDC : 0;
    if (q) func_001F37C8(p, func_001C0D70(func_001A62E8(p), 1));
    d = p->g130;
    if (d != 0.0f) {
        float v = p->g12C + D_002DDED0.dt * d;
        p->g12C = v;
        if (v <= 0.0f) {
            p->g12C = 0.0f;
            p->g130 = 0.0f;
            p->pvt->a[9].pfn((char *)p + p->pvt->a[9].d);
        } else if (1.0f <= v) {
            p->g12C = 1.0f;
            p->g130 = 0.0f;
        }
    }
}
