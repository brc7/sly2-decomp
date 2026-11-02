struct E { char pad[0x14]; int id; };
struct S { unsigned short c; short pad; struct E *a; char pad8[0x20]; int x28; };
void func_00217C20(struct S *p, int i);
/** @sly2-annot addr=0x217BA0
 * Purpose: find anim index by id (0x314 -> 0) and start
 * Info: proposed name StartSpriteAnimById (T2); TU hud
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00217BA0(struct S *p, int k)
{
    int i;
    if (p->x28) {
        i = 0;
        if (k != 0x314) {
            for (i = 0; i < p->c; i++)
                if (k == p->a[i].id) break;
            if (i >= p->c) i = 0;
        }
        func_00217C20(p, i);
    }
}
