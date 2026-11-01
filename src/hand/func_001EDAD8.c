struct SAI { int a; };
struct SHDANIM { char pad[0x24]; short du; short dv; int pad28; int fDirty; char pad2[0x34 - 0x30]; struct SAI sai; };
int FIsSlEmpty(struct SAI *psai);
static inline int IFromG(float g) { return (int)(g * 4096.0f + 0.5f) & 0xFFF; }
void func_001EDAD8(struct SHDANIM *p, float du, float dv)
{
    int u, v;
    if (FIsSlEmpty(&p->sai)) return;
    u = IFromG(du);
    v = IFromG(dv);
    if (p->du == u && p->dv == v) return;
    p->du = u;
    p->dv = v;
    p->fDirty = 1;
}
