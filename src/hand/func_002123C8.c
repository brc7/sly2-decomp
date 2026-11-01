struct VT { char pad[0x188]; short d; short pd; void (*pf)(void *, int); };
struct S {
    char pad[0xC]; struct VT *pvt; char pad10[0x30]; unsigned int x40; char pad44[0x29]; unsigned char x6D;
    char pad6e[0x26]; int x94; int x98; float x9C; float xA0; char pada4[4]; int xA8; char padac[0x10];
    int xBC; char padc0[8]; struct S *xC8;
};
extern struct G { char pad[0x3BE8]; void *x3BE8; } D_003DD490;
void PostLoLoad(struct S *p);
void *PloFindSwObject__FP2SWi3OIDP2LO(struct G *, int, int, struct S *);
void SetLoBusyk(struct S *p, int);
void func_002123C8(struct S *p)
{
    unsigned int v;
    float w;
    int h;
    PostLoLoad(p);
    if (p->xBC) {
        struct G *pg = &D_003DD490;
        if (!pg->x3BE8)
            pg->x3BE8 = PloFindSwObject__FP2SWi3OIDP2LO(pg, 0x405, 0x374, p);
    }
    p->xC8 = p;
    p->xA8 = -1;
    h = (p->x94 << 9) / 640;
    w = (float)p->x98 * 0.45454547f;
    p->xA0 = 2048.0f - w * 0.5f;
    p->x9C = 2048.0f - (float)h * 0.5f;
    if (p->x6D)
        p->pvt->pf((char *)p + p->pvt->d, -1);
    SetLoBusyk(p, 1);
    v = p->x40;
    if ((v ^ 1) & 1) *(int *)v |= 0x102;
    else p->x40 = v | 0x102;
}
