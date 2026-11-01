struct E { char pad[0x14]; int id; };
struct H { char pad[8]; short h; char pada[0xE]; };
struct A { unsigned short c; unsigned short c2; struct E *ae; struct H *ah; char padc[0x24]; };
struct S { char pad[0x80]; unsigned short c80; short pad82; struct A *aa; };
void func_00218038(struct A *pa, float g);
void func_00218060(struct A *pa, int l, float g, int z);
/** @sly2-annot addr=0x212F80
 * Purpose: like SetWidgetAnimPhase but absolute time
 * Info: proposed name SetWidgetAnimTime (T2); TU hud
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00212F80(struct S *p, int k, float g)
{
    int i, j;
    if (k == 0x314) {
        func_00218038(p->aa, g);
        return;
    }
    for (i = 0; i < p->c80; i++) {
        struct A *a = &p->aa[i];
        for (j = 0; j < a->c; j++) {
            struct E *e = &a->ae[j];
            int l = -1;
            if (k == e->id) {
                int m;
                for (m = 0; m < a->c2; m++)
                    if (a->ah[m].h != -1)
                        l = m;
                if (l >= 0)
                    func_00218060(a, l, g, 0);
            }
        }
    }
}
