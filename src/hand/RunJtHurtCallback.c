struct PMF { short delta; unsigned short idx; void (*pfn)(void *); };
struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { int x0, x4, x8; struct VTE *vt; };
struct B { char pad[0x148]; unsigned long x148; };
struct S { struct O *x0; int x4; char pad[0x14]; struct PMF pmf; char pad2[0xC]; struct B *x30; };
void *func_002333B4(void *p, int c, unsigned int n);
void ReturnJtToSafePlaceCb(struct S *p);
void func_00195A00(void *p, int a, int b, int c);
void RunJtHurtCallback(struct S *s)
{
    struct PMF pmf = s->pmf;
    func_002333B4(&s->pmf, 0, sizeof(s->pmf));
    if (pmf.idx) {
        pmf.pfn((char *)s + pmf.delta);
    } else if ((unsigned int)(s->x4 - 8) < 5) {
        struct O *o = s->x0;
        struct VTE *e = &o->vt[0x2E8 / 8];
        e->pfn((char *)o + e->d);
    } else if (s->x30 && (s->x30->x148 & 0x200000000UL)) {
        ReturnJtToSafePlaceCb(s);
    } else {
        func_00195A00((char *)s->x0 + 0x2C0, 11, 0, 0);
    }
}
