struct F { int pad[5]; void (*pfn)(struct F *); int pad2[2]; };
struct E { int n; struct F *af; int x8; int xC; int x10; int x14; int x18; };
struct S { int pad[9]; int x24; struct E a[16]; int x1E8; int x1EC; };
void SetPauseMenuState(struct S *s, int n);
void *func_002333B4(void *p, int c, unsigned int n);
int func_001B0108(struct S *s);
/** @sly2-annot addr=0x1AF4D8
 * Purpose: Push page (items, count, widget, title) onto menu stack x24, run item init callbacks, state 1
 * Info: proposed name PushPauseMenuPage (T2); TU menu
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
void func_001AF4D8(struct S *s, int n, struct F *af, int x8, int x18)
{
    struct E *e;
    int i;
    if (!x8) {
        if (s->x24 > 0)
            x8 = s->a[s->x24 - 1].x8;
    }
    e = &s->a[s->x24++];
    func_002333B4(e, 0, sizeof(*e));
    e->af = af;
    e->n = n;
    e->x8 = x8;
    e->x18 = x18;
    e->xC = 0;
    SetPauseMenuState(s, 1);
    for (i = 0; i < e->n; i++) {
        struct F *f = &e->af[i];
        if (f->pfn)
            f->pfn(f);
    }
    if (func_001B0108(s)) {
        s->x1EC = s->x24;
        s->x24--;
    }
}
