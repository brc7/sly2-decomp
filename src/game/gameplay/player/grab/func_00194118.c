typedef struct T { char pad[0xFC]; int fc, x100, x104, x108, x10c, x110, x114; int pad2[2]; int x120, x124, x128; int x12c; int x130; int p3[3]; int x140; int p4[8]; int x164, x168, x16c; } T;
typedef struct O { char pad[0x68]; T *t; } O;
extern char D_002BF670[];
/** @sly2-annot addr=0x194118
 * Purpose: Reset a block of TN parameters (x100..x16C) of the grab context's private TN to defaults
 * Info: proposed name ResetJtGrabTnParams (T2); TU grab
 * Notes: names_v2 C proposed ResetJtDynamicTunnel__FP2JT (0.241)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/player.md#grab-0x191090-0x195950
 * @end-sly2-annot */
void func_00194118(O *o)
{
    T *t = o->t;
    t->x100 = 0;
    t->fc = 0;
    t->x108 = 0;
    t->x164 = 5;
    t->x10c = 0;
    t->x110 = 1;
    t->x120 = 0;
    t->x124 = 0;
    t->x16c = 0;
    t->x114 = (int)D_002BF670;
    t->x130 = 0;
    t->x168 = 1;
    t->x128 = 3;
    t->x140 = 0;
}
