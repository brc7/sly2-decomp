typedef int qword __attribute__((mode(TI)));
struct V3 { float x, y, z; };
struct Y14 { char pad[0x78]; float f78; };
struct Y { char pad[0x14]; struct Y14 *p14; };
struct X { char pad[0x40]; void *p40; };
struct Z { char pad[0x30]; int k30; char pad2[0x40 - 0x34]; int n40; float f44; };
struct S { char pad[0x190]; struct V3 v190; char pad2[0xFBC - 0x19C]; int k; float t; void *pFC4; };
extern struct { float t; float dt; } D_002DDED0;
extern qword D_002A2DE0;
extern qword D_002C3D30;
extern float D_002A8698;
extern char D_003DD490[];
int func_00206EB0(struct S *p, int k);
void func_0012D108(void *p, qword q);
void func_00208728(struct S *p, int n);
void func_00206E50(struct S *p);
void func_00208700(struct S *p, int n);
void func_00206DB0(struct S *p);
void *func_00205458(struct S *p, int n);
struct Y *func_00209518(void *p);
void func_00131A20(struct Y *y, float f);
int func_001C4840(void *p);
void func_00177BA0(struct Z *z);

/** @sly2-annot addr=0x207030
 * Purpose: Gadget/status override state +0xFBC (time +0xFC0): 8/0xD shrink (scale act -> 0.5), 0xE
 *   time-frozen (BHV157 ASEGA speed 0, velocity 0), 0x22/0x25 rage (target other guards: prefer cid 0x38,
 *   ignore 5 cids), 0x15 (BHV156 sub-states); undoes the old state
 * Info: proposed name SetStepguardSpecial (T1); TU stepguard
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7b_guard
 * See: docs/engine/guards-ai.md#4-combat-ko-and-takedowns, docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00207030(struct S *p, int k)
{
    int old;
    if (k == p->k)
        return;
    if (!func_00206EB0(p, k))
        return;
    old = p->k;
    p->k = k;
    p->t = D_002DDED0.t;
    switch (old) {
    case 8:
    case 0xD:
        if (p->pFC4)
            func_0012D108(p->pFC4, D_002A2DE0);
        break;
    case 0x22:
    case 0x25:
        func_00208728(p, 0x38);
        func_00206E50(p);
        break;
    case 0xE: {
        struct X *x = func_00205458(p, 0x9D);
        if (x) {
            struct Y *y = func_00209518(x->p40);
            if (y)
                func_00131A20(y, y->p14->f78);
        }
        break;
    }
    case 0x15: {
        struct Z *z = func_00205458(p, 0x9C);
        if (z && z->k30 == 0x12 && z->n40 == func_001C4840(D_003DD490) && z->f44 == D_002A8698)
            func_00177BA0(z);
        break;
    }
    }
    switch (k) {
    case 8:
    case 0xD:
        if (p->pFC4)
            func_0012D108(p->pFC4, D_002C3D30);
        break;
    case 0x22:
    case 0x25:
        func_00208700(p, 0x38);
        func_00206DB0(p);
        break;
    case 0xE: {
        struct X *x = func_00205458(p, 0x9D);
        if (x) {
            struct Y *y = func_00209518(x->p40);
            if (y)
                func_00131A20(y, 0.0f);
        }
        {
            struct V3 *pv = &p->v190;
            pv->z = 0;
            pv->y = 0;
            pv->x = 0;
        }
        break;
    }
    case 0x15: {
        struct Z *z = func_00205458(p, 0x9C);
        if (z) {
            switch (z->k30) {
            case 1: case 2: case 3: case 4:
            case 0xC: case 0x10: case 0x11: case 0x12:
                func_00177BA0(z);
                break;
            }
        }
        break;
    }
    }
}
