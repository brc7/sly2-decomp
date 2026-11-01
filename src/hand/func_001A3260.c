typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
static __inline__ qword Make(float x, float y, float z)
{
    qword q, t;
    __asm__("mfc1 %0,%2
	mfc1 %1,%3
	pextlw %0,%1,%0
	mfc1 %1,%4
	pcpyld %0,%1,%0"
            : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    return q;
}
struct LO;
typedef struct { short d; short pad; void (*pfn)(struct LO *, M3 *); } VT;
struct LO { int x0, x4, x8; VT *vt; char pad[0x20 - 0x10]; struct LO *x20; char pad2[0x160 - 0x24]; int x160; };
extern struct LO *D_003E04F8;
extern qword D_002A2DC0;
struct LO *func_0013E3C8(int a, int b, int c, int d);
void func_0011B830(M3 *pm, qword v, qword w);
void func_001A3610(struct LO *p, float f);
void func_001A3630(struct LO *p, float f);
/** @sly2-annot addr=0x1A3260
 * Purpose: After world load, if no light in the world light list D_003E04F8 qualifies (x160==0), create a
 *   LIGHT (PloNew CID 4, oid 0x188) aimed along (-0.2,0.3,-1) with strength 255 and angle 240
 * Info: proposed name EnsureSwDefaultLight (T1); TU light
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/hud-ui.md#4-source-files, docs/engine/rendering.md#12-source-files,
 *   docs/reference/functions/rendering.md#light-0x1a3260-0x1a4e08
 * @end-sly2-annot */
void func_001A3260(void)
{
    struct LO *p;
    M3 c;
    union { qword q; float f; } r;
    M3 m;
    for (p = D_003E04F8; p; p = p->x20) {
        if (p->x160 == 0) break;
    }
    if (p) return;
    p = func_0013E3C8(4, 0, 0x188, -1);
    r.q = Make(-0.2f, 0.3f, -1.0f);
    func_0011B830(&m, r.q, D_002A2DC0);
    c = m;
    p->vt[30].pfn((struct LO *)((char *)p + p->vt[30].d), &c);
    func_001A3610(p, 255.0f);
    func_001A3630(p, 240.0f);
}
