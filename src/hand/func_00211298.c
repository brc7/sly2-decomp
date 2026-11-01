struct RESOURCE { int id; const char *text; };
struct G { char gap[0x4640]; int count; struct RESOURCE *resources; };
extern struct G D_003DD490;
struct F { int id; char pad[0x1C]; };
struct E { int c; struct F *af; char pad[0xC]; int x14; char pad2[4]; };
extern struct E D_003D9EDC[];
extern int D_003D9EF4;
const char *func_0013E368(struct G *g, int id);
char *func_0021ED58(char *dst, const char *src);
/** @sly2-annot addr=0x211298
 * Purpose: STRINGGEN ids 0x2D2-0x2DB: name of entry (k + sel - 3) mod n of the current menu page
 *   D_003D9EDC[D_003D9EF4]
 * Info: proposed name BuildMenuCarouselText (T2); TU hud
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00211298(char *dst, int k)
{
    struct E *e = &D_003D9EDC[D_003D9EF4];
    if (e->c) {
        int n = e->c;
        int j = k + (long)(e->x14 - 3);
        while (j < 0)
            j += n;
        (void)func_0021ED58(dst, func_0013E368(&D_003DD490, e->af[j % n].id));
    }
}
