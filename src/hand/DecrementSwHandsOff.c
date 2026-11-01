struct S { char pad[0x3D78]; int n; };
struct G;
extern struct G D_002DFC00;
void RemoveGrfusr__Fi(int n);
void func_00187E08(struct G *pg, int n);
void ReleaseSwLetterbox(struct S *p);
void func_0020F1A8(struct S *p);
void DecrementSwHandsOff(struct S *p, int grf)
{
    if (--p->n == 0) {
        RemoveGrfusr__Fi(2);
        func_00187E08(&D_002DFC00, 8);
    }
    if (grf & 1) ReleaseSwLetterbox(p);
    if (grf & 2) func_0020F1A8(p);
}
