struct S { char pad[0x150]; int f150; };
struct M { int f0; struct S *ps; };
void HandleBasicMessage(struct S *, int, struct M *);
void func_0013B5C8(struct S *, float);
void func_0013AAE8(struct S *p, int msg, struct M *pm) {
    HandleBasicMessage(p, msg, pm);
    if (msg == 4 && pm->ps == p && p->f150 == 0) func_0013B5C8(p, 0.0f);
}
