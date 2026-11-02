struct MSG { int cb; int nKind; };
struct L;
struct C { char pad[6]; unsigned char b6; char pad7; struct L *p8; char pad2[0x14 - 0xC]; };
struct ENT { int n; void *p; };
struct L {
    char pad[4];
    unsigned char b4;
    unsigned char b5;
    char pad6[2];
    int c8;
    int cC;
    int c10;
    char pad14[3];
    unsigned char b17;
    struct C *ac;
    int n1C;
    struct ENT *ae;
    int n24;
    void *p28;
};
extern void **D_003DD004;
void func_00117968(void *, int, void *);
void ColorCorrectRgba(int *);
int func_00117D50(void *);
void *func_001AE2A0(struct MSG *);
void LoadTexFromBrx(void *, struct C *);
static inline void InitEnt(struct ENT *pe, int n, struct MSG *pmsg)
{
    pe->n = n;
    pmsg->cb = n * 16;
    pmsg->nKind = 0x10;
    pe->p = func_001AE2A0(pmsg);
}
/** @sly2-annot addr=0x1EB2F0
 * Purpose: Read one shader: header, colors (color-corrected), texture list (LoadTexFromBrx) and per-frame
 *   data
 * Info: proposed name LoadShdFromBrx (T1); TU shd
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels |
 *   textures format doc cites this function by this name (CONFIRMED(code) in the doc)
 * Confidence: high [T1 0.80] - evidence: static analysis; sources:
 *   r6b_rest+parse-verified+format-doc:textures
 * See: docs/engine/rendering.md#13-key-functions,
 *   docs/formats/level-objects.md#2-per-world-entry-header-loop-body-of-loadswfrombrx,
 *   docs/formats/textures.md#1-layout-inside-a-world-entry-cstatic--cparse
 * @end-sly2-annot */
void LoadShdFromBrx(void *pbis, struct L *p)
{
    struct MSG msg;
    int i, j;
    struct ENT *ae, *pe;
    func_00117968(pbis, 0x18, p);
    ColorCorrectRgba(&p->c8);
    ColorCorrectRgba(&p->cC);
    ColorCorrectRgba(&p->c10);
    i = func_00117D50(pbis);
    if (i >= 0)
        p->p28 = D_003DD004[i];
    msg.cb = p->b17 * 0x14;
    msg.nKind = 0x10;
    p->ac = func_001AE2A0(&msg);
    for (j = 0; j < p->b17; j++) {
        struct C *pc = &p->ac[j];
        LoadTexFromBrx(pbis, pc);
        pc->p8 = p;
    }
    p->n24 = p->ac->b6;
    switch (p->b4) {
    case 0:
    case 4:
    case 5:
    case 6:
        p->n1C = 1;
        break;
    case 1:
    case 2:
        p->n1C = 2;
        break;
    case 7:
        p->n1C = (p->b5 & 0x20) ? 2 : 3;
        break;
    case 3:
        p->n1C = 4;
        break;
    }
    msg.cb = p->n1C * 8;
    msg.nKind = 0x10;
    ae = func_001AE2A0(&msg);
    p->ae = ae;
    switch (p->b4) {
    case 0:
    case 4:
    case 5:
    case 6:
        InitEnt(ae, p->ac->p8->n24 + 8, &msg);
        break;
    case 3:
    {
        int n = (p->b5 & 2) ? p->ac->p8->n24 + 2 : 0;
        InitEnt(ae, n + 6, &msg);
        pe = &ae[1];
        InitEnt(pe, 6, &msg);
        pe = &ae[2];
        InitEnt(pe, 8, &msg);
        pe = &ae[3];
        InitEnt(pe, 4, &msg);
        break;
    }
    case 2:
        InitEnt(ae, 5, &msg);
        pe = &ae[1];
        InitEnt(pe, 5, &msg);
        break;
    case 1:
    {
        int n = p->ac->p8->n24 + 8;
        InitEnt(ae, n, &msg);
        pe = &ae[1];
        InitEnt(pe, n, &msg);
        break;
    }
    case 7:
    {
        int n = p->ac->p8->n24 + 8;
        InitEnt(ae, n, &msg);
        pe = &ae[1];
        InitEnt(pe, n, &msg);
        pe = &ae[2];
        if (!(p->b5 & 0x20))
            InitEnt(pe, n, &msg);
        break;
    }
    }
}
