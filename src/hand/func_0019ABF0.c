typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } V;
struct CONTEXT; struct OBJECT;
struct T { struct CONTEXT *a; struct OBJECT *b; int c, d; qword q; };
struct S { struct CONTEXT *a; struct OBJECT *b; };
void func_001E6F10(struct OBJECT *, float, V *, V *);
qword ConvertAloPos(struct OBJECT *, struct OBJECT *, qword);
float func_0019ABB8(void *, float);
void func_0011D9E0(float (*)(void *, float), void *, float, float, float, float, float *, float *);
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
typedef char V_Size[sizeof(V)==16?1:-1];
typedef char T_Size[sizeof(struct T)==32?1:-1];
typedef char S_Size[sizeof(struct S)==8?1:-1];
typedef char S_Context_Offset[FIELD_OFFSET(struct S,a)==0?1:-1];
typedef char S_Object_Offset[FIELD_OFFSET(struct S,b)==4?1:-1];
typedef char T_Context_Offset[FIELD_OFFSET(struct T,a)==0?1:-1];
typedef char T_Object_Offset[FIELD_OFFSET(struct T,b)==4?1:-1];
typedef char T_Point_Offset[FIELD_OFFSET(struct T,q)==16?1:-1];
float func_0019ABF0(struct S *s, float parameter) {
    V v; struct T t; float r;
    struct OBJECT *b = s->b;
    func_001E6F10(b, parameter, &v, 0);
    t.a = s->a;
    t.b = b;
    t.q = ConvertAloPos(0, b, v.q);
    func_0011D9E0(func_0019ABB8, &t, 0.5f, 0.25f, 0.25f, 3.0f, 0, &r);
    return r;
}
