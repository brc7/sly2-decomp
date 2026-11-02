// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31
typedef struct { char pad[0x30]; short d; short p; void (*f)(void *, void *); } VT;
typedef struct { int *data; int c; } ARR;
typedef struct { char pad[0xC]; VT *vt; ARR arr; } S;
void CDynArray_Append(ARR *, int, int);
int *func_002565E0(S *p, int x)
{
    ARR *a = &p->arr;
    CDynArray_Append(a, x, 1);
    p->vt->f((char *)p + p->vt->d, &p->arr.data[a->c - 1]);
    return &p->arr.data[a->c - 1];
}
