// Forward a sprite frame update with its duration and colors.
struct RGBA2 { unsigned char r, g, b, a; };
struct RGBA {
    unsigned char r, g, b, a;
    RGBA(const RGBA &c) { *(RGBA2 *)this = *(const RGBA2 *)&c; }
} __attribute__((aligned(16)));
struct L { char pad[0x84]; char *pb; };
struct S { int x0; int k; L *pl; int xc; int i; };
struct T { char pad[0x58]; int x58; };
extern "C" void func_002183E8(void *p, int a, void *b, float g, RGBA c0, RGBA c1);
/** @sly2-annot addr=0x215850
 * Purpose: forward frame update (time, M23, colours) to nested sprite
 * Info: proposed name UpdateSpriteItem (T2); TU hud
 * Notes: names_v2 v1 proposed FUN_001aea08 (0.000)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
extern "C" void func_00215850(float g, S *p, T *t, void *b, RGBA c0, RGBA c1)
{
    if (p->k == 4)
        func_002183E8(p->pl->pb + p->i * 0x30, t->x58, b, g, c0, c1);
}
