typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct V { char pad[0x20]; qword q20; };
struct T { char pad[0x35C]; struct V *p35C; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x3C]; int n50; int pad3; float g58; };
void UpdateBhvanim(struct S *p);
void func_0017A5B0(struct S *p, int n);
qword func_001A62E8(void *p);
void func_0017A670(struct S *p) {
    int s;
    UpdateBhvanim(p);
    do {
        s = p->n50;
        if (s == 0 && D_002DDED0.t > p->g58) func_0017A5B0(p, 1);
    } while (p->n50 != s);
    { struct T *t = p->pt; struct V *v = t->p35C; v->q20 = func_001A62E8(t); }
}
