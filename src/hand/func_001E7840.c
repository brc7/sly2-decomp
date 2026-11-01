struct P { char pad[0x40]; int fl; char pad2[0x17C - 0x44]; void *px; };
void *func_001AE4F8(int cb);
void *InitChain(void *pv, struct P *p);
void MeasureShape(struct P *p);
void SetLoBusyk(struct P *p, int n);
void *func_001E7840(struct P *p, int f)
{
    if (!p->px) {
        p->px = InitChain(func_001AE4F8(0x90), p);
        MeasureShape(p);
        SetLoBusyk(p, 0);
        { int v = p->fl; if ((v ^ 1) & 1) *(int *)v &= ~0x20; else p->fl = v & ~0x20; }
        { int v = p->fl; if ((v ^ 1) & 1) *(int *)v |= 0xC; else p->fl = v | 0xC; }
    } else if (f == 1) {
        SetLoBusyk(p, 0);
        { int v = p->fl; if ((v ^ 1) & 1) *(int *)v |= 0x4; else p->fl = v | 0x4; }
    }
    return p->px;
}
