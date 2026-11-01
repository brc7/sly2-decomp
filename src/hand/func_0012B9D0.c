typedef int qword __attribute__((mode(TI)));
typedef struct VECTOR3 { float x, y, z; } VECTOR3;
typedef union V3Q { VECTOR3 v; qword q; } V3Q;
typedef struct KEY { VECTOR3 pos; float t; qword dposIn; qword dposOut; } KEY;
typedef struct T { int pad; int c; KEY *a; } T;
int func_0012B6D8(T *t, float g);
void EvaluateBezierPos(float tMax, float t, float s, qword a, qword va, qword b, qword vb, void *ppos, void *pv, int n);
void func_0012B9D0(T *t, void *pv, void *pdv, float g, float dtScale)
{
    int i = func_0012B6D8(t, g);
    V3Q u0, u1;
    u0.v = t->a[i - 1].pos;
    u1.v = t->a[i].pos;
    EvaluateBezierPos(t->a[i].t - t->a[i - 1].t, g - t->a[i - 1].t, dtScale, u0.q, *(qword *)((char *)&t->a[i] - 0x10), u1.q, t->a[i].dposIn, pv, pdv, 0);
}
