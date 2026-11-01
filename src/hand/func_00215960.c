struct L { char pad[0x84]; char *pb; };
struct S { int x0; int k; struct L *pl; int xc; int i; };
struct T { char pad[0x58]; int x58; char pad5c[5]; unsigned char b61; };
void func_002185F8(void *p, int a, int b);
void func_0011A098(void *p, struct T *t);
/** @sly2-annot addr=0x215960
 * Purpose: draw item: nested sprite drawn recursively, other kinds prepended to draw list
 * Info: proposed name QueueSpriteItem (T2); TU hud
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00215960(struct S *p, struct T *t, int b)
{
    switch (p->k) {
    case 4:
        func_002185F8(p->pl->pb + p->i * 0x30, t->x58, t->b61);
        break;
    case 1:
    case 3:
    case 6:
    case 7:
        t->b61 = b;
        func_0011A098((char *)p->pl + 0xFC, t);
        break;
    case 2:
    case 5:
        break;
    }
}
