struct E { int mask; unsigned char ch; };
struct T { struct E a[16]; };
extern struct T D_002A97F8;
extern char D_002A9878[];
struct S { int x0, x4, flags; };
void sprintf(char *pch, char *fmt, int ch);
/** @sly2-annot addr=0x227940
 * Purpose: appends one char per set bit from a 16-entry {mask,ch} table D_002A97F8 via sprintf
 * Info: proposed name FlagsToPchz (T2); TU tu_2275B8
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88,
 *   docs/reference/functions/sound.md#tu_2275b8-0x2275b8-0x227bb8
 * @end-sly2-annot */
void func_00227940(struct S *p, char *pch)
{
    struct T t = D_002A97F8;
    unsigned int i;
    *pch = 0;
    for (i = 0; i < 16; i++) {
        if (p->flags & t.a[i].mask)
            sprintf(pch, D_002A9878, t.a[i].ch);
    }
}
