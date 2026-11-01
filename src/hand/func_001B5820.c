struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern char D_003DD490[];
extern int D_002C0908[][5], D_002C0948[][5];
extern float D_002C0900;
struct P { char pad[0x150]; int x150; char pad1[0x1034 - 0x154]; int x1034; char pad2[0x1524 - 0x1038]; float f1524; float f1528; int x152C; int x1530; int x1534; };
struct P *func_001C4840(void *g);
int func_0020F0B0(void *g);
int func_001F07E8(int a);
int func_001F07C8(int a);
int func_001C46C0(int a, int b, float f);
void func_001F0660(int a, int b);
/** @sly2-annot addr=0x1B5820
 * Purpose: State 6 (scripted SMA struggle): only when Murray is current PO and not hands-off; face-button
 *   mashing (0xF0) decrements counter; picks next SMA state 0x1CA..0x1D0 from tables
 *   D_002C0908/D_002C0948[x1534] and drives both own SMA (x1034) and partner SMA (x1530)
 * Info: proposed name UpdateMurrayStruggle (T2); TU murray
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_001B5820(struct P *p)
{
    int a;
    if (func_001C4840(D_003DD490) != p) return;
    if (func_0020F0B0(D_003DD490)) return;
    a = func_001F07E8(p->x1034);
    if (a != func_001F07C8(p->x1034)) return;
    if (a >= 0x1D0) return;
    if (a < 0x1CB) return;
    p->f1524 -= D_002DDED0.dt;
    if (func_001C46C0(p->x150, 0xF0, 0.0f)) {
        p->x152C--;
        p->f1528 = p->f1524;
    }
    if (p->f1524 <= 0.0f) {
        if (p->x152C <= 0) a = D_002C0908[p->x1534][a - 0x1CB];
        else a = D_002C0948[p->x1534][a - 0x1CB];
    } else if (p->f1528 - p->f1524 > D_002C0900) {
        a = 0x1CA;
    }
    if (p->x1530) func_001F0660(p->x1530, a);
    func_001F0660(p->x1034, a);
}
