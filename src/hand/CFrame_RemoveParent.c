typedef struct T { int a; int b; int c; void *ap[1]; } T;
void func_00256EE0(void *pp, int n);
void func_001AE5A0(void *pvDst, void *pvSrc, unsigned int cb);
void CFrame_RemoveParent(T *p, void *pv)
{
    int i;
    for (i = 0; i < p->c; i++) {
        if (p->ap[i] == pv) {
            func_00256EE0(&p->ap[i], 0);
            p->c--;
            func_001AE5A0(p->ap[i], p->ap[i + 1], (p->c - i) * 4);
            return;
        }
    }
}
