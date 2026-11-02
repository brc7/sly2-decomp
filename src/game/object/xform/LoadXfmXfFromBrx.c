// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31
typedef struct { char pad[0x188]; short d; short p; int (*f)(void *, int); } VT;
typedef struct { char pad[0xC]; VT *vt; char pad2[0x40]; int x50; char pad3[0xC]; char x60[1]; } S;
int S16Read__18CBinaryInputStream(void *); void ReadMatrix4__18CBinaryInputStreamP7MATRIX4(void *, void *); void func_002313F0(S *); int U32Read__18CBinaryInputStream(void *); void func_001A6D28(S *, int);
void LoadXfmXfFromBrx(S *p, void *q)
{
    int v = S16Read__18CBinaryInputStream(q);
    p->x50 = p->vt->f((char *)p + p->vt->d, v);
    ReadMatrix4__18CBinaryInputStreamP7MATRIX4(q, p->x60);
    func_002313F0(p);
    func_001A6D28(p, U32Read__18CBinaryInputStream(q));
}
