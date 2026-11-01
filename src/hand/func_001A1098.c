struct E { int x0; int x4; int x8; };
struct S { int x0; int pad[4]; int x14; struct E *x18; };
struct D { int a; int b; };
extern char D_002BFA30[];
struct E *func_001AE238(struct D *d, void *p);
int func_001F4C58(int a, int b, int c);
/** @sly2-annot addr=0x1A1098
 * Purpose: Allocate a 7-entry table (PvAllocSwCopyImpl 0x54 x 0x10) and look up hurt-related child XFs
 *   (PxfFindSo)
 * Info: proposed name PostLoadJtHurt (T2); TU hurt
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#hurt-0x1a0b18-0x1a2bd0
 * @end-sly2-annot */
void func_001A1098(struct S *s)
{
    struct D d;
    int i;
    s->x14 = 7;
    d.a = 0x54;
    d.b = 0x10;
    s->x18 = func_001AE238(&d, D_002BFA30);
    for (i = 0; i < s->x14; i++) {
        struct E *e = &s->x18[i];
        e->x8 = func_001F4C58(s->x0, e->x0, 6);
    }
}
