extern int D_003E1C54;
extern int D_003E1C30;
struct L { int a[4]; };
struct M { int a; int b; int c[2]; };
void func_00117988(void *q, int n);
void func_001AE478(struct L *pl);
void *func_001AE318(struct M *pm);
void func_00117968(void *q, int a, void *x);
void func_001EA568(void *x);
void func_00170730(void *p);
void func_001AE4A0(struct L *pl, int n);
void func_001179B8(void *q, int a);
void func_00170828(void *p);
void LoadSwSharedDataFromBrx(void *p, void *q)
{
    func_00117988(q, 0x10);
    if (D_003E1C54) {
        struct L l;
        struct M m;
        void *x;
        func_001AE478(&l);
        { struct M *pm = &m;
        pm->a = D_003E1C30;
        pm->b = 0x10;
        x = func_001AE318(pm); }
        func_00117968(q, D_003E1C30, x);
        func_001EA568(x);
        func_00170730(p);
        func_001AE4A0(&l, 2);
    } else {
        func_001179B8(q, D_003E1C30);
        func_00170828(p);
    }
}
