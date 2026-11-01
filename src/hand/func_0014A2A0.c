struct P { char pad[0x10]; void *x10; int pad1; void *x18; char pad2[0x30 - 0x1C]; int x30; int pad3[2]; int x3C; int x40; int x44; };
extern char D_003DD490[];
void func_001EEF98(struct P *p);
void func_0014A4D0(struct P *p);
int func_001EF0A8(struct P *p);
int func_00209098(void *p);
int func_0014A478(struct P *p);
int func_0014AA30(struct P *p);
int func_001C4840(void *p);
int func_001758A0(void *p, int n, int m);
void func_0014AE38(struct P *p, int k);
void func_0014AD70(struct P *p);
int func_00178D00(void *p);
int func_00174FB8(void *p);
int func_00175748(void *p);
/** @sly2-annot addr=0x14A2A0
 * Purpose: BHV182 slot 10: keep pace with the player along a route (distance-to-player logic 0x14A4D0, next
 *   point 0x14AD70, nearest point 0x14AA30)
 * Info: proposed name UpdateBhv182 (T2); class BHV182 slot10; TU tu_14A008
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/guards-ai.md#tu_14a008-0x14a008-0x14b660
 * @end-sly2-annot */
void func_0014A2A0(struct P *p)
{
    int s;
    func_001EEF98(p);
    func_0014A4D0(p);
    for (;;) {
        if (func_001EF0A8(p))
            break;
        s = p->x30;
        switch (s) {
        case 0:
            if (func_00209098(p->x10)) {
                func_0014AE38(p, 4);
            } else if (func_0014A478(p)) {
                func_0014AE38(p, 2);
            } else {
                int v = func_0014AA30(p);
                p->x40 = v;
                if (v == p->x3C) {
                    int r = func_001758A0(p->x10, func_001C4840(D_003DD490), 0);
                    p->x44 = r;
                    if (r)
                        func_0014AE38(p, 3);
                } else {
                    func_0014AD70(p);
                }
            }
            break;
        case 1:
            if (func_00209098(p->x10))
                func_0014AE38(p, 4);
            else if (func_0014A478(p))
                func_0014AE38(p, 2);
            else if (func_00178D00(p->x18))
                func_0014AE38(p, 0);
            else
                func_001758A0(p->x10, func_001C4840(D_003DD490), 0);
            break;
        case 2:
            if (func_00209098(p->x10))
                func_0014AE38(p, 4);
            else if (func_00174FB8(p->x18))
                func_0014AE38(p, 0);
            break;
        case 3:
            if (func_00209098(p->x10))
                func_0014AE38(p, 4);
            else if (func_00175748(p->x18))
                func_0014AE38(p, 0);
            break;
        case 4:
            if (!func_00209098(p->x10))
                func_0014AE38(p, 0);
            break;
        }
        if (s == p->x30)
            break;
    }
}
