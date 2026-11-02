// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31
typedef struct { int pad[4]; short d; short p; void (*f)(void *); } VT;
typedef struct { int pad[3]; VT *vt; } O;
typedef struct { char pad[0x484]; O *x484; O *x488; char pad2[0x154]; O *x5E0; O *x5E4; } S;
void func_00258348(S *, int);
void func_00258290(S *p, int x)
{
    if (p->x5E4) p->x5E4->vt->f((char *)p->x5E4 + p->x5E4->vt->d);
    if (p->x5E0) p->x5E0->vt->f((char *)p->x5E0 + p->x5E0->vt->d);
    if (p->x488) p->x488->vt->f((char *)p->x488 + p->x488->vt->d);
    if (p->x484) p->x484->vt->f((char *)p->x484 + p->x484->vt->d);
    func_00258348(p, x);
}
