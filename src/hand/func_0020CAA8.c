struct M { int a, b, n; };
struct S { char pad[0x34]; int f34; int f38; };
void HandleBasicMessage(struct S *p, int msg, struct M *pm);
void func_0020CAA8(struct S *p, int msg, struct M *pm)
{
    if (msg == 9) {
        switch (pm->n) {
        case 0x17D: p->f34 = 1; break;
        case 0x17E: p->f34 = 0; break;
        case 0x17F: p->f38 = 1; break;
        case 0x180: p->f38 = 0; break;
        }
    }
    HandleBasicMessage(p, msg, pm);
}
