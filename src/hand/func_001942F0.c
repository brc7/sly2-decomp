typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
struct S4C { char pad[0x148]; long fl; };
struct A { char pad[0x40]; int x40; char pad2[0x4C-0x44]; struct S4C *x4c; char pad3[0x60-0x50]; qword x60; char pad4[0x88-0x70]; float x88; };
struct O { char pad[0x208]; struct A *x208; char pad2[0x9E0-0x20C]; char x9e0[0x108C-0x9E0]; int x108c; };
struct T { char pad[0xEC]; int xec; char pad2[0x10C-0xF0]; float x10c; char pad3[0x184-0x110]; float x184; char pad4[0x19C-0x188]; int x19c; };
struct P { struct O *x0; int x4; int x8; int pad[2]; struct T *x14; void *x18; int x1c; float x20; };
int func_00132B10(struct T *t, int n);
void func_0019FD30(void *p, void *s, float g);
void func_0019FEB0(void *p, int a, qword q);
qword func_001A62E8(void *p);
float func_002458F0(float y, float x);
/** @sly2-annot addr=0x1942F0
 * Purpose: Per grab state, record the held spot/param into the STEPPLAYER+0x9E0 place table (0x19FD30 /
 *   0x19FEB0) unless the object forbids it
 * Info: proposed name RecordJtGrabPlace (T2); TU grab
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#grab-0x191090-0x195950
 * @end-sly2-annot */
void func_001942F0(struct P *p)
{
    float g, s;
    switch (p->x8) {
    case 3:
        if (p->x14->x19c)
            return;
        g = p->x20;
        func_0019FD30(p->x0->x9e0, p->x18, g);
        break;
    case 4:
        if (p->x14->xec)
            return;
        g = 0.0f;
        func_0019FD30(p->x0->x9e0, p->x18, g);
        break;
    case 0:
    case 13:
        if (func_00132B10(p->x14, 0xC)) {
            if (p->x14->x19c)
                return;
            g = p->x20;
            func_0019FD30(p->x0->x9e0, p->x18, g);
            break;
        }
        if (func_00132B10(p->x14, 0x26)) {
            if (p->x14->xec)
                return;
            g = 0.0f;
            func_0019FD30(p->x0->x9e0, p->x18, g);
            break;
        }
        if (func_00132B10(p->x14, 0x22)) {
            if (p->x14->xec)
                return;
            s = p->x14->x10c;
        } else
            s = p->x14->x184;
        {
            qword a;
            U d;
            a = func_001A62E8(p->x0);
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(d.q) : "j"(a), "j"(func_001A62E8(p->x14)));
            g = func_002458F0(d.f[1], d.f[0]) * s;
        }
        func_0019FD30(p->x0->x9e0, p->x18, g);
        break;
    default:
    {
        struct O *o = p->x0;
        struct A *pa = o->x208;
        if (pa && pa->x88 > 0.7f && !(pa->x4c->fl & 0x20000000) && !o->x108c)
            func_0019FEB0(o->x9e0, pa->x40, pa->x60);
        return;
    }
    }
}
