struct E { char pad[0x14]; int id; };
struct A { unsigned short c; short pad; struct E *ae; char pad8[0x28]; };
struct S { char pad[0x7C]; int x7C; unsigned short c80; short pad82; struct A *aa; };
int func_00218738(struct A *a);
int func_00218710(struct A *a);
/** @sly2-annot addr=0x2121E8
 * Purpose: is anim id (0x314=main track) still running on its sprite
 * Info: proposed name FWidgetAnimPlaying (T1); TU hud
 * Notes: names_v2 v1 proposed PostCtrLoad__FP3CTR (0.005)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
int func_002121E8(struct S *p, int k)
{
    int j, l;
    if (k == -1)
        return 0;
    if (p->x7C == -1)
        return 0;
    if (k == 0x314)
        return func_00218738(p->aa) == 0;
    for (j = 0; j < p->c80; j++) {
        struct A *a = &p->aa[j];
        for (l = 0; l < a->c; l++) {
            if (k == a->ae[l].id)
                return func_00218710(a);
        }
    }
    return 0;
}
