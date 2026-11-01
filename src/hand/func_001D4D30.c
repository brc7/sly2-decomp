typedef int qword __attribute__((mode(TI)));
typedef struct { int a, b; } S8;
typedef struct {
    char pad0[0x10];
    void *x10;
    char pad14[0x40 - 0x14];
    S8 x40;
    S8 x48;
    qword x50;
    S8 x60;
    float x68;
    char pad6c[0x88 - 0x6C];
    float x88;
    float x8C;
    int x90;
    float x94;
    float x98;
    float x9C;
} OBJ;
extern S8 D_002C2218, D_002C2220, D_002C2240;
extern qword D_002C2230;
/** @sly2-annot addr=0x1D4D30
 * Purpose: Weapon record defaults (owner, default vectors D_002C2218..)
 * Info: proposed name InitRobottankWeapon (T2); TU tu_1D1370
 * Notes: names_v2 v1 proposed UpdateRemit__FP5REMITf (0.013) | class identity from whole_game WE names/object
 *   strings: InitRcCarWeapon -> InitRobottankWeapon
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r5b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150
 * @end-sly2-annot */
void func_001D4D30(OBJ *p, void *q)
{
    p->x10 = q;
    p->x40 = D_002C2218;
    p->x48 = D_002C2220;
    p->x50 = D_002C2230;
    p->x60 = D_002C2240;
    p->x68 = -1750.0f;
    p->x88 = 200.0f;
    p->x8C = 400.0f;
    p->x90 = 0;
    p->x94 = 500.0f;
    p->x98 = 1.0f;
    p->x9C = 1.5f;
}
