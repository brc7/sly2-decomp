struct L { char pad[0x84]; char *pb; };
struct Q { char pad[0x20]; void *x20; };
struct S { int x0; int k; struct L *pl; int xc; int i; struct Q *x14; };
struct T { char pad[0x58]; int x58; };
void func_00218660(void *p, int a);
void func_001BED18(void *p);
void InitSpriteItem(struct T *t);
/** @sly2-annot addr=0x2158C0
 * Purpose: release item payload (stop nested slot / texture ref) and reinit
 * Info: proposed name ReleaseSpriteItem (T2); TU hud
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_002158C0(struct S *p, struct T *t)
{
    switch (p->k) {
    case 4:
        func_00218660(p->pl->pb + p->i * 0x30, t->x58);
        break;
    case 6:
        if (p->x14) {
            if (p->x14->x20)
                func_001BED18(p->x14->x20);
            p->x14 = 0;
        }
        break;
    }
    InitSpriteItem(t);
}
