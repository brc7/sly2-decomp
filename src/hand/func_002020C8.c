typedef int qword __attribute__((mode(TI)));
struct T { char pad[0x28]; unsigned long fl28; };
struct O { char pad[0x100]; struct T *pt; float r; };
struct S { char pad[0x30]; int c; struct O *a[1]; };
extern struct { char pad[0x90]; qword pos; } D_002DDF40;
qword func_001A62E8(void *p);
void FadeSoIn(struct T *pt, float f);
void FadeSoOut(struct T *pt, float f);

void func_002020C8(struct S *p)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct O *o = p->a[i];
        qword a, d;
        float f, r;
        if (!o->pt)
            continue;
        a = func_001A62E8(o);

        __asm__("vsub.xyzw %0, %1, %2\n\tvmul.xy %0, %0, %0\n\tvaddy.x %0, %0, %0y\n\tvsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(a), "j"(D_002DDF40.pos));
        __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
        if (o->r < f && !((o->pt->fl28 >> 60) & 1))
            FadeSoIn(o->pt, 0.25f);
        else if (f <= o->r && !!((o->pt->fl28 >> 60) & 1))
            FadeSoOut(o->pt, 0.25f);
    }
}
