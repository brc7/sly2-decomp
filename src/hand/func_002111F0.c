struct G;
extern struct G D_003DD490;
struct Z { char pad[0xF8]; int xF8; };
struct Y { char pad[0x74]; int c; struct Z *a[1]; };
struct X { char pad[0xF38]; struct Y *py; };
struct X *func_001C4840(struct G *g);
int func_00132B10(void *p, int cid);
char *func_0013E368(struct G *g, int id);
void func_0021ED58(char *dst, char *src);
/** @sly2-annot addr=0x2111F0
 * Purpose: STRINGGEN ids 0x2DF-0x2E4: langstr of recon target i (P+0x78[i]->idName F8) if i < P+0x74
 * Info: proposed name BuildReconTargetText (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_002111F0(char *dst, int i)
{
    struct G *g = &D_003DD490;
    struct X *p = func_001C4840(g);
    if (p && func_00132B10(p, 0x34)) {
        struct Y *y = p->py;
        if (y && i < y->c)
            func_0021ED58(dst, func_0013E368(g, y->a[i]->xF8));
    }
}
