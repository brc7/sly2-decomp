struct E { int x0, x4; void *x8; int xc, x10; char *x14; int x18; };
struct G { int pad[9]; int x24; };
extern struct E D_003D9EDC[];
extern struct G D_003D9ED0;
extern int D_002E6594;
struct S { int x0, x4; char *x8; };
void func_001AF608(struct G *g);
int func_001B0108(struct G *g);
void func_00213B00(void *a, char *b);
/** @sly2-annot addr=0x1AD740
 * Purpose: Menu hook: set text id 0x5C8+i or 0x5C5+i for slot i
 * Info: proposed name SetSaveSlotLabel (T2); TU memcard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001AD740(struct S *s)
{
    struct E *e;
    if (D_002E6594 != 1) {
        func_001AF608(&D_003D9ED0);
        return;
    }
    if (!func_001B0108(&D_003D9ED0))
        return;
    e = &D_003D9EDC[D_003D9ED0.x24];
    if (e->x14 == s->x8)
        func_00213B00(e->x8, s->x8 + 0x5C8);
    else
        func_00213B00(e->x8, s->x8 + 0x5C5);
}
