typedef unsigned long ulong __attribute__((mode(DI)));
struct GIFS;
struct FONT { char pad[0x3C]; int n3C; ulong q40; int pad48[2]; ulong q50; };
void AddPrimPack__4GIFSiiUl(struct GIFS *, int, int, int);
void PackAD__4GIFSll(struct GIFS *, int, ulong);
void AddPrimList__4GIFSiUl(struct GIFS *, int, ulong);
int FEnsureLoaded(struct FONT *, struct GIFS *);
/** @sly2-annot addr=0x15F220
 * Purpose: CFontBrx::SetupDraw: SCISSOR from the text box rectangle, FEnsureLoaded, TEXFLUSH if needed, then
 *   the sprite prim list setup
 * Info: proposed name SetupDraw (T1); TU font
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/hud-ui.md#font-0x15ef00-0x1603b8
 * @end-sly2-annot */
void SetupDraw(struct FONT *pfont, float *prc, struct GIFS *pgifs)
{
    int fFlush;
    if (prc) {
        int x1, y1, x0, y0;
        x1 = (int)((prc[0] + prc[2] * 512.0f) * 0.0015625f) - 1;
        y1 = (int)((prc[1] + prc[3]) * 0.45454547f) - 1;
        x0 = (int)(prc[0] * 0.8f);
        y0 = (int)(prc[1] * 0.45454547f);
        AddPrimPack__4GIFSiiUl(pgifs, 0, 1, 0xE);
        PackAD__4GIFSll(pgifs, 0x40, (ulong)x0 | ((ulong)x1 << 16) | ((ulong)y0 << 32) | ((ulong)y1 << 48));
    }
    fFlush = FEnsureLoaded(pfont, pgifs);
    AddPrimPack__4GIFSiiUl(pgifs, 6, 1, 0xE);
    if (fFlush)
        PackAD__4GIFSll(pgifs, 0x3F, 0);
    PackAD__4GIFSll(pgifs, 0x47, pfont->n3C ? pfont->q40 : 0x31001);
    PackAD__4GIFSll(pgifs, 0x42, 0x44);
    PackAD__4GIFSll(pgifs, 6, pfont->q50);
    PackAD__4GIFSll(pgifs, 0x14, 0x60);
    PackAD__4GIFSll(pgifs, 8, 5);
    PackAD__4GIFSll(pgifs, 0, 0x1D6);
    AddPrimList__4GIFSiUl(pgifs, 5, 0x43431);
}
