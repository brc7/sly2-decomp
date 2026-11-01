typedef int qword __attribute__((mode(TI)));
struct S { char pad[0x18]; int x18; };
extern qword D_002A2DE0;
void PostLoadStepplayer(struct S *p);
void PostJtLoadGait(void *p);
void func_001A0300(struct S *p, void *a, void *b);
void func_0020BD88(void *p, struct S *s);
void PostJtthrowLoad(void *p);
void func_001426E8(struct S *p, int n);
void *PxfFindSo(struct S *p, int a, int b);
int func_0012DC18(struct S *p, void *a, int b, int c, int d, qword q);
void SetMurrayState(struct S *p, int n);
void PostMurrayLoad(struct S *p)
{
    char *b = (char *)p;
    void *v;
    PostLoadStepplayer(p);
    PostJtLoadGait(b + 0x1320);
    PostJtLoadGait(b + 0x1460);
    PostJtLoadGait(b + 0x1630);
    PostJtLoadGait(b + 0x1540);
    func_001A0300(p, b + 0x16F0, b + 0x17F0);
    func_0020BD88(b + 0x1850, p);
    PostJtthrowLoad(b + 0x1910);
    func_001426E8(p, 0x16);
    v = PxfFindSo(p, p->x18, 0xC);
    if (v) {
        *(int *)(b + 0x1ACC) = func_0012DC18(p, v, 0, 7, 2, D_002A2DE0);
    }
    SetMurrayState(p, 0);
}
