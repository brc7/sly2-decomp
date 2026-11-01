typedef struct REC { int id; float a, b, c, d; } REC;
typedef struct H { void *p; } H;
typedef struct T { char pad[0xE4]; int c; REC *arec; H *ap; } T;
void OnLoAdd(T *t);
void *func_001FBB90(int id);
void func_001FB2F8(void *p, T *t);
void func_001FB4F0(void *p, float a, float b);
void func_001FB1F8(void *p, float c);
void func_001FB2C0(void *p, float d);
void func_00258040(H *pp, void *p);
void func_0012ED20(T *t)
{
    int i;
    OnLoAdd(t);
    for (i = 0; i < t->c; i++) {
        REC *prec = &t->arec[i];
        void *p = func_001FBB90(prec->id);
        func_001FB2F8(p, t);
        func_001FB4F0(p, prec->a, prec->b);
        func_001FB1F8(p, prec->c);
        func_001FB2C0(p, prec->d);
        func_00258040((H *)(i * 4 + (int)t->ap), p);
    }
}
