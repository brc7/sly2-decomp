#include "vec.h"
typedef int qword __attribute__((mode(TI)));
struct S4C { char pad[0x148]; long fl; };
struct A { char pad[0x40]; int x40; char pad2[0x4C-0x44]; struct S4C *x4c; char pad3[0x60-0x50]; qword x60; char pad4[0x88-0x70]; float x88; };
struct B { char pad[0x10C]; int x10c; };
struct P { char pad[0x208]; struct A *x208; char pad2[0x108C-0x20C]; int x108c; char pad3[0x18C4-0x1090]; struct B *x18c4; int x18c8; };
extern struct VECTOR D_002A2DD0;
void func_0019FEB0(void *p, int a, qword q);
void func_0019FD30(void *p, void *b, float g);
void func_001942F0(void *p);
void func_0019EA60(void *p);
/** @sly2-annot addr=0x19FC30
 * Purpose: Each frame, when standing on stable ground (jts in a ground list, floor normal z>0.7, not flagged)
 *   record the floor object/pos; grab and pipe states record their spot (RecordJtGrabPlace/RecordJtPipePlace)
 * Info: proposed name UpdateJtPlaces (T1); TU places
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#places-0x19f3e8-0x1a0300
 * @end-sly2-annot */
void func_0019FC30(struct P *p)
{
    struct VECTOR v = D_002A2DD0;
    switch (p->x18c8) {
    case 0: case 1: case 5: case 6: case 9: case 10: case 11: case 12:
    case 15: case 16: case 17: case 20: case 23: case 24:
    {
        struct A *pa = p->x208;
        if (pa && pa->x88 > 0.7f && !(pa->x4c->fl & 0x20000000) && !p->x108c)
            func_0019FEB0((char *)p + 0x9E0, pa->x40, pa->x60);
        break;
    }
    case 3: case 22:
    {
        struct B *pb = p->x18c4;
        if (!pb->x10c)
            func_0019FD30((char *)p + 0x9E0, pb ? (char *)pb + 0x100 : 0, 0.0f);
        break;
    }
    case 7:
        func_001942F0((char *)p + 0x4C0);
        break;
    case 8:
        func_0019EA60((char *)p + 0xF50);
        break;
    }
}
