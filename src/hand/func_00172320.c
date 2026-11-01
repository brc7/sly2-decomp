typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern float D_002A5398;
struct VTE { short d; short pad; void (*pfn)(void *); };
struct X { qword a; qword v; };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x2C]; int n3C; char pad3[0xC0 - 0x40]; int nC0; };
struct A { char pad[0xFD0]; char aFD0[4]; };
struct S { char pad[0x10]; struct A *p10; int pad2; char a18[0x18]; int n30; float f34; float f38; char pad3[0xC]; int n48; };
void func_001EF0D8(void *p);
struct O *func_00205438(void *p, int n);
qword func_00172760(struct S *p);
qword func_001A62E8(void *p);
void func_002562A8(struct X *x);
void func_0017DD38(void *p, struct X *x);
float func_002458F0(float y, float x);
void func_00209818(void *p, float f);
void func_00172560(struct S *p);
void func_001728B0(struct S *p);
void func_00172F10(struct S *p);
void func_001725E0(struct S *p, int n);
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
/** @sly2-annot addr=0x172320
 * Purpose: Enters a search state, starting the matching sub-behaviour (BHV157 anim 0xC/0xE/1, BHV186 goto,
 *   BHV158 look) and timers
 * Info: proposed name SetBhv171State (T2); TU tu_171F58
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_171f58-0x171f58-0x173158
 * @end-sly2-annot */
void func_00172320(struct S *p, int n) {
    if (n == p->n30) return;
    func_001EF0D8(p->a18);
    p->n30 = n;
    p->f34 = D_002DDED0.t;
    switch (n) {
    case 0:
        { struct O *o = func_00205438(p->p10, 0x9D); struct VTE *e; o->n3C = 0xC; e = &o->vt[8]; e->pfn((char *)o + e->d); }
        break;
    case 1:
        {
            struct O *o = func_00205438(p->p10, 0xBA);
            struct X x;
            struct VTE *e;
            qword v;
            o->nC0 = 1;
            v = func_00172760(p);
            func_002562A8(&x);
            x.v = v;
            func_0017DD38(o, &x);
            e = &o->vt[8]; e->pfn((char *)o + e->d);
            p->f38 = D_002A5398;
        }
        break;
    case 2:
        {
            qword a = func_00172760(p);
            qword b = func_001A62E8(p->p10);
            qword d;
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
            func_00209818(p->p10->aFD0, func_002458F0(GetY(d), GetX(d)));
            { struct O *o = func_00205438(p->p10, 0x9E); struct VTE *e = &o->vt[8]; e->pfn((char *)o + e->d); }
            func_00172560(p);
        }
        break;
    case 3:
        {
            struct O *o = func_00205438(p->p10, 0xBA);
            struct VTE *e;
            o->nC0 = 1;
            func_001728B0(p);
            func_00172F10(p);
            e = &o->vt[8]; e->pfn((char *)o + e->d);
        }
        break;
    case 4:
        { struct O *o = func_00205438(p->p10, 0x9D); struct VTE *e; o->n3C = 0xE; e = &o->vt[8]; e->pfn((char *)o + e->d); }
        func_001725E0(p, 0);
        p->n48 = 0;
        break;
    case 5:
        { struct O *o = func_00205438(p->p10, 0x9E); struct VTE *e; o->n3C = 1; e = &o->vt[8]; e->pfn((char *)o + e->d); }
        break;
    }
}
