// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31
typedef int qword __attribute__((mode(TI)));
typedef struct { char pad[0x50]; int x50; char pad2[0xC]; float m[12]; char x90[0xC]; float x9C; } S;
qword func_0022FC48(int, int, qword); void VECTOR_opAssign(void *, qword); void func_002313F0(S *);
void TranslateXfmToPos(S *p, qword v)
{
    float *m = p->m;
    qword r = func_0022FC48(0, p->x50, v);
    VECTOR_opAssign((char *)p + 0x90, r);
    m[15] = 1.0f;
    func_002313F0(p);
}
