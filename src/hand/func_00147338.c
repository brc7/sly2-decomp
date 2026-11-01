struct MSG { int a; int b; int k; };
struct P { char pad[0x160]; int id; int f164; int f168; int f16C; int x170; };
void HandleBasicMessage(struct P *p, int kind, struct MSG *pmsg);
void func_00147410(struct P *p, int n);
void func_00147338(struct P *p, int kind, struct MSG *pmsg)
{
    HandleBasicMessage(p, kind, pmsg);
    if (kind != 9) return;
    if (pmsg->a != p->id) return;
    switch (pmsg->k) {
    case 0x5FB: p->f164 = 1; break;
    case 0x5FC: p->f164 = 0; break;
    case 0x5FD: p->f168 = 1; break;
    case 0x5FE: p->f168 = 0; break;
    case 0x5FF: p->f16C = 0; break;
    case 0x600: p->f16C = 1; break;
    case 0x601:
        if (p->x170 != -1) func_00147410(p, 3);
        break;
    }
}
