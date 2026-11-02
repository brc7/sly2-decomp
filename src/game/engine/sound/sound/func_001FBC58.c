struct VTE { short d; short p; void (*pfn)(void *); };
struct P { char pad[0xC]; struct VTE *pvt; char pad1[0x14 - 0x10]; int n14; float f18; char pad2[0x44 - 0x1C]; float f44; char pad3[0x50 - 0x48]; unsigned short f50; unsigned short n52; char pad4[0x60 - 0x54]; int n60; };
struct G { char pad[0xE8]; int nE8; };
struct S { int n0; };
struct M { int n0; int pad4; int n8; int nC; short h10; short pad12; int pad14[3]; short h20; short pad22; int pad24; int n28; int pad2C; };
extern struct G D_003DD040;
extern char D_003DD490[];
extern int D_002DD5BC, D_002F7900, D_002E1E40;
struct S *func_001FD028(struct G *, int);
short func_001FD070(struct G *, struct S *);
void func_001FB658(struct P *, int, int);
void func_001FB710(struct P *);
void *func_002333B4(void *, int, int);
void func_001FBAF8(void);
void func_00232320(struct M *, void (*)(void), struct P *);
void func_002322F0(struct M *);
int PpoHero(void *);
void func_001FEED0(struct G *);
/** @sly2-annot addr=0x1FBC58
 * Purpose: vt9: build 989 play params (bank, sound index, vol/pan/pitch); loopers play async with handle
 *   callback, one-shots fire-and-forget; per-sfx special cases
 * Info: proposed name PlayBanksnd (T1); class BANKSND; TU sound
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FBC58(struct P *p)
{
    struct G *pg = &D_003DD040;
    struct S *ps = func_001FD028(pg, p->n60);
    struct M m;
    int id;
    if (!ps || !pg->nE8) {
        func_001FB658(p, 0x1FFF, 0);
        return;
    }
    if (p->f50 & 2) {
        func_001FB710(p);
        return;
    }
    func_002333B4(&m, 0, 0x30);
    m.n28 = 3;
    m.n0 = pg->nE8;
    m.h10 = func_001FD070(pg, ps);
    if (p->f50 & 0x10) {
        m.n28 |= 4;
        m.n8 = (int)(p->f44 * 1024.0f);
    }
    if (p->f50 & 0x20) {
        m.n28 |= 8;
        m.nC = p->n14;
    }
    if (p->f50 & 0x40) {
        m.n28 |= 0x10;
        m.h20 = (int)(p->f18 * 1524.0f);
    }
    if (ps->n0 == 2) {
        p->n52 = 1;
        p->pvt[1].pfn((char *)p + p->pvt[1].d);
        func_00232320(&m, func_001FBAF8, p);
        func_001FB658(p, 0x7F, 0);
        return;
    }
    id = 0;
    func_002322F0(&m);
    func_001FB658(p, 0x1FFF, 0);
    switch (p->n60) {
    case 0x6F: id = D_002E1E40; break;
    case 0x73: id = D_002DD5BC; break;
    case 0x15: id = D_002F7900; break;
    }
    if (id == PpoHero(D_003DD490)) func_001FEED0(&D_003DD040);
}
