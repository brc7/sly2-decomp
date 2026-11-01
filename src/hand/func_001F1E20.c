struct E { int n; int pad[2]; int nC; };
struct P { char pad[0x404]; struct E ae[8]; int c484; int i488; int pad2[2]; void *p494; };
struct G;
extern struct G D_002DEB30;
void BroadcastGameEvent(struct G *, int, struct P *);
void *func_001C2EF0(void *, int);
void func_001F17F8(struct P *, int);
void func_001F1CF8(struct P *);
void *func_001F1E20(struct P *p)
{
    void *r;
    if (p->i488 >= p->c484) return 0;
    {
        BroadcastGameEvent(&D_002DEB30, 0x19, p);
        r = func_001C2EF0(p->p494, (p->ae + p->i488)->nC);
        func_001F17F8(p, -1);
        p->i488++;
        func_001F1CF8(p);
    }
    return r;
}
