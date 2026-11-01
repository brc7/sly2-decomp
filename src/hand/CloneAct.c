typedef struct T { int a; int b; int c; int d; int e; int f; int g; char ab[1]; } T;
typedef struct G { int pad[3]; unsigned int cb; } G;
extern G D_003E1230;
void CopyAb__FPvT0Ui(void *dst, void *src, unsigned int cb);
void CloneAct(T *t, T *src, int b, int c)
{
    t->a = src->a;
    t->d = src->d;
    t->f = src->f;
    t->b = b;
    t->c = c;
    CopyAb__FPvT0Ui(t->ab, src->ab, D_003E1230.cb - 0x1C);
}
