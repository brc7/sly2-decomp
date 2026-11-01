struct Q { char pad[0x4C]; int n; };
struct P { char pad[0x40]; int fl; char pad2[0x17C - 0x44]; struct Q *px; };
void *func_001E7840(struct P *p, int f);
void SetLoBusyk(struct P *p, int n);
void SetShapeChainMode(struct P *p, int n)
{
    if (n != -1) {
        func_001E7840(p, 1);
        SetLoBusyk(p, 0);
        { int v = p->fl; if ((v ^ 1) & 1) *(int *)v &= ~0x20; else p->fl = v & ~0x20; }
        { int v = p->fl; if ((v ^ 1) & 1) *(int *)v |= 0xC; else p->fl = v | 0xC; }
    } else {
        SetLoBusyk(p, 0);
        { int v = p->fl; if ((v ^ 1) & 1) *(int *)v |= 0x20; else p->fl = v | 0x20; }
        { int v = p->fl; if ((v ^ 1) & 1) *(int *)v &= ~0xC; else p->fl = v & ~0xC; }
    }
    if (p->px) p->px->n = n;
}
