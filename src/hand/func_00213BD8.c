struct E { char pad[0x14]; int id; };
struct A { unsigned short c; short pad; struct E *ae; char pad8[0x28]; };
struct S { char pad[0x80]; unsigned short c80; short pad82; struct A *aa; };
void func_00214100(struct S *p, int j, int l, int x, int y, int z);
/** @sly2-annot addr=0x213BD8
 * Purpose: start first anim id match with argument x
 * Info: proposed name PlayWidgetAnimArg (T2); TU hud
 * Notes: names_v2 C proposed DxFromPchz__5CFontPc (0.438)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00213BD8(struct S *p, int k, int x)
{
    int j, l;
    for (j = 0; j < p->c80; j++) {
        struct A *a = &p->aa[j];
        for (l = 0; l < a->c; l++) {
            struct E *e = &a->ae[l];
            if (k == e->id) {
                func_00214100(p, j, l, x, 0, 0);
                return;
            }
        }
    }
}
