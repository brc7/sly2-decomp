typedef int ti128 __attribute__((mode(TI)));
struct Q { char pad[0x148]; unsigned long grf; };
struct A { char pad[0x40]; int f40; char pad1[8]; struct Q *pq; char pad2[0x60 - 0x50]; ti128 v60; char pad3[0x88 - 0x70]; float f88; };
struct S { char pad[0x208]; struct A *pa; char pad1[0x4c0 - 0x20c]; int f4c0; char pad2[0x9e0 - 0x4c4]; int f9e0;
    char pad3[0xf50 - 0x9e4]; int ff50; char pad4[0x108c - 0xf54]; int f108c; char pad5[0x13d0 - 0x1090]; int st; };
void func_0019FEB0(void *, int, ti128);
void func_001942F0(void *);
void func_0019EA60(void *);
/** @sly2-annot addr=0x135EB0
 * Purpose: Per state: remember standing places (jtplaces 0x19FEB0) when on solid ground, or grab/pipe updates
 * Info: proposed name UpdateBentleyPlaces (T2); TU bentley
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
void func_00135EB0(struct S *p) {
    struct A *pa;
    switch (p->st) {
    case 0: case 1: case 5: case 6: case 7: case 11: case 12: case 14:
        pa = p->pa;
        if (pa && 0.7f < pa->f88 && !(pa->pq->grf & 0x20000000) && !p->f108c)
            func_0019FEB0(&p->f9e0, pa->f40, pa->v60);
        break;
    case 3:
        func_001942F0(&p->f4c0);
        break;
    case 13:
        func_0019EA60(&p->ff50);
        break;
    }
}
