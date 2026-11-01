struct M { int n0; int pad4; int n8; int nC; short h10; short pad12; int pad14[3]; short h20; short pad22; int pad24; int n28; int pad2C; };
struct P { char pad[0x64]; int n64; char pad2[0xE8 - 0x68]; int nE8; char pad3[0x138 - 0xEC]; int n138; };
int NRandInRange__Fii(int, int);
void *func_002333B4(void *, int, int);
void *func_001FD028(struct P *, int);
short func_001FD070(struct P *, void *);
void func_002322F0(struct M *);
void func_001FEED0(struct P *p)
{
    int r;
    if (p->n64) return;
    {
    int a[7][6] = {
        { -5, -5, -5, -5, -5, -5 },
        { -5, -5, -5, -5, -5, 1 },
        { -5, -5, -5, -5, 1, 1 },
        { -5, -5, -5, 1, 1, 1 },
        { -5, -5, 1, 1, 1, 1 },
        { -5, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1 },
    };
    int b[25] = { 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0 };
    struct M m;
    r = NRandInRange__Fii(0, 5);
    p->n138 += a[b[p->n138 + 12]][r];
    func_002333B4(&m, 0, 0x30);
    m.n28 = 0x13;
    m.n0 = p->nE8;
    m.h10 = func_001FD070(p, func_001FD028(p, 0x28));
    m.h20 = p->n138 * 127;
    func_002322F0(&m);
    }
}
