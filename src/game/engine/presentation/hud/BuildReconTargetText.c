struct G;
extern struct G D_003DD490;
struct Z { char pad[0xF8]; int xF8; };
struct Y { char pad[0x74]; int c; struct Z *a[1]; };
struct X { char pad[0xF38]; struct Y *py; };
struct X *PpoHero(struct G *g);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int cid);
char *PchzFindSwLangstr__FP2SWi(struct G *g, int id);
void strcpy(char *dst, char *src);
/** @sly2-annot addr=0x2111F0
 * Purpose: STRINGGEN ids 0x2DF-0x2E4: langstr of recon target i (P+0x78[i]->idName F8) if i < P+0x74
 * Info: proposed name BuildReconTargetText (T1); TU hud
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void BuildReconTargetText(char *dst, int i)
{
    struct G *g = &D_003DD490;
    struct X *p = PpoHero(g);
    if (p && FIsBasicDerivedFrom__FP5BASIC3CID(p, 0x34)) {
        struct Y *y = p->py;
        if (y && i < y->c)
            strcpy(dst, PchzFindSwLangstr__FP2SWi(g, y->a[i]->xF8));
    }
}
