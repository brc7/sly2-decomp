struct X { char pad[0xF20]; int xF20; char padf24[0x18]; float xF3C; };
extern char D_003DD490[];
extern float D_002DE3B8, D_002DE514;
extern char D_002A8AB8[], D_002A8AC0[], D_002A8AC8[], D_002A8AD0[], D_002A8AD8[];
struct X *PpoHero(void *);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct X *, int);
int func_0021ECD8(char *, char *, ...);
int func_0023E290(float);
/** @sly2-annot addr=0x210FE0
 * Purpose: STRINGGEN id 0x2DC: per-character readout from STEPPLAYER F20/F3C: JT "x%d", BENTLEY "x%5.3f",
 *   MURRAY "Far"/"Near"/"YIKES"
 * Info: proposed name BuildBinocReadoutText (T2); TU hud
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/engine/player.md#8-look-mode-recon-mode-and-the-ai-partner,
 *   docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00210FE0(char *pch)
{
    struct X *x = PpoHero(D_003DD490);
    float g;
    if (!x || !FIsBasicDerivedFrom__FP5BASIC3CID(x, 0x34))
        return;
    switch (x->xF20) {
    case 0:
        g = D_002DE3B8 * x->xF3C;
        break;
    case 1:
        g = D_002DE514 * x->xF3C;
        break;
    }
    if (FIsBasicDerivedFrom__FP5BASIC3CID(x, 0x35)) {
        func_0021ECD8(pch, D_002A8AB8, (int)g);
    } else if (FIsBasicDerivedFrom__FP5BASIC3CID(x, 0x37)) {
        func_0021ECD8(pch, D_002A8AC0, func_0023E290(g));
    } else if (FIsBasicDerivedFrom__FP5BASIC3CID(x, 0x36)) {
        int n = (int)g;
        if (n < 4)
            func_0021ECD8(pch, D_002A8AC8, n);
        else if (n < 8)
            func_0021ECD8(pch, D_002A8AD0, n);
        else
            func_0021ECD8(pch, D_002A8AD8, n);
    }
}
