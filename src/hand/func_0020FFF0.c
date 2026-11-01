typedef struct { short d; short pad; void *pfn; } VTE;
struct Y { char pad[0xC]; VTE *vt; };
struct D { int a; int b; int c; int d; };
int CploFindSwObjects_15E650(void *p, int k, int a, int b, int c, void **ppo);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *o, int n);
struct Y *func_001F67E0(void *o, int b);
void SubscribeLoObject(struct Y *y, int x);
void SeekSma(struct Y *y, int id);
void SetSmaGoal(struct Y *y, int id);
void func_0020FFF0(void *p, struct D *pd, int b, int fl, int x, struct Y **ppy)
{
    void *o;
    struct Y *y;
    int id;
    if (ppy)
        *ppy = 0;
    if (CploFindSwObjects_15E650(p, 0x204, pd->a, b, 1, &o) != 1)
        return;
    if (!FIsBasicDerivedFrom__FP5BASIC3CID(o, 0x30))
        return;
    y = func_001F67E0(o, pd->b);
    if (!y)
        return;
    if (fl & 1)
        id = pd->c;
    else
        id = pd->d;
    if (id == -1)
        return;
    if (x) {
        *ppy = y;
        ((void (*)(void *))y->vt[1].pfn)((char *)y + y->vt[1].d);
        SubscribeLoObject(y, x);
    }
    if (fl & 2)
        SeekSma(y, id);
    else
        SetSmaGoal(y, id);
}
