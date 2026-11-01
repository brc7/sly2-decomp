struct C { char pad[0xE4]; int xe4; };
struct S { int a, b, c; struct C *pc; int d, e; int x18; };
float SMinShape(struct C *);
float SMaxShape(struct C *);
void func_0019E110(struct S *p, float *ag) {
    if (p->pc->xe4) {
        ag[0] = -3.40282346638528859812e+38f;
        ag[1] = 3.40282346638528859812e+38f;
    } else if (p->x18) {
        ag[0] = SMinShape(p->pc) + 35.0f;
        ag[1] = SMaxShape(p->pc) - 100.0f;
    } else {
        ag[0] = SMinShape(p->pc) + 100.0f;
        ag[1] = SMaxShape(p->pc) - 35.0f;
    }
}
