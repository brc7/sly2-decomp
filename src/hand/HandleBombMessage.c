struct S { char pad[0x150]; int f150; };
struct M { int f0; struct S *ps; };
void HandleBasicMessage(struct S *, int, struct M *);
void PrimeBomb__FP4BOMBf(struct S *, float);
void HandleBombMessage(struct S *p, int msg, struct M *pm) {
    HandleBasicMessage(p, msg, pm);
    if (msg == 4 && pm->ps == p && p->f150 == 0) PrimeBomb__FP4BOMBf(p, 0.0f);
}
