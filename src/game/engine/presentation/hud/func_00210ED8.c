struct CRef { int tagk; int tag; };
extern char D_003D4A60[];
extern char D_002A8AA0[];
void func_002581C0(struct CRef *r, int *p, void *q);
void func_00129EC8(struct CRef *r);
void fiprintf(void *f, const char *fmt, ...);
/** @sly2-annot addr=0x210ED8
 * Purpose: STRINGGEN id 0x2CF: sprintf("%d", saved var loot.ccoin) = HUD coin counter text
 * Info: proposed name BuildCoinCountText (T1); TU hud
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud
 * See: docs/engine/hud-ui.md#1-widget-hud-objects-hud-tu-0x210eb0-0x21bb88-file-name-unknown,
 *   docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00210ED8(void *f, int **pp)
{
    int *p = *pp;
    if (p && *p) {
        char *fmt = D_002A8AA0;
        struct CRef r;
        int v;
        func_002581C0(&r, p, D_003D4A60);
        v = r.tag;
        func_00129EC8(&r);
        fiprintf(f, fmt, v);
    }
}
