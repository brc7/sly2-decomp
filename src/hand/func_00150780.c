typedef int qword __attribute__((mode(TI)));
typedef union VU { qword q; float f[4]; } VU;
struct G { char pad[0x90]; qword q90; };
extern struct G D_002DDF40;
extern char D_003DD490[];
struct S { int pad[4]; void *p10; };
qword func_001A62E8(void *p);
void *PpoCur(void *p);
qword func_00150780(struct S *p)
{
    VU v;
    if (p->p10) {
        v.q = func_001A62E8(p->p10);
    } else {
        void *o = PpoCur(D_003DD490);
        if (o) {
            v.q = func_001A62E8(o);
            v.f[2] += 150.0f;
        } else {
            v.q = D_002DDF40.q90;
        }
    }
    return v.q;
}
