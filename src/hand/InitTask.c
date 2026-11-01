struct S { char pad[0x54]; int f54; int f58; int f5c; char p2[0xB8-0x60]; int fb8; int fbc; int fc0; char p3[0xF0-0xC4]; int ff0; int ff4; };
struct G { char pad[0x3AFC]; int c; };
extern struct G D_003DD490;
void InitLo(struct S *p);
void InitTask(struct S *p)
{
    InitLo(p);
    p->f54 = -1;
    p->ff0 = -1;
    p->f5c = -1;
    p->fb8 = -1;
    p->fbc = -1;
    p->fc0 = -1;
    p->ff4 = 0;
    D_003DD490.c++;
}
