typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pf)(); };
struct LO { char pad[0xC]; struct VTE *vt; };
struct O { char pad[0x30]; qword q30; char p1[0xB0 - 0x40]; qword qB0; };
struct S {
    char pad[0xC]; struct VTE *vt;
    char p0[0x58 - 0x10]; struct O *x58;
    char p1[0x150 - 0x5C]; int x150; float x154; int p158; int x15C; float x160; void *x164; struct LO *x168; struct LO *x16C;
    qword q170; qword q180; qword q190;
    char p2[0x1CC - 0x1A0]; float x1CC;
};
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern qword D_002A2DD0;
extern qword D_002C0D60;
extern float D_002A773C;
void SetLoParent_1A58D0(struct S *p, int a, int b);
struct LO *func_001CB658(struct S *p);
void UnlockProxyPlo(struct LO *q, struct S *p);
struct LO *func_001F0380(void *a, struct S *p, int anim, int z);
void func_001F10F0(struct LO *q, float g);
void func_001F0660(struct LO *q, int anim);
void func_001F0598(struct LO *q);
void func_001C60B8(struct S *p, int n)
{
    int anim = -1;
    struct LO *q;
    if (p->x150 == n) return;
    if (n != 0 && n != 5)
        SetLoParent_1A58D0(p, 0, 0);
    if (p->x150 == -1 || p->x150 == 2) {
        struct O *o = p->x58;
        p->q170 = o->q30;
        p->q180 = o->qB0;
    }
    switch (n) {
    case 0:
        anim = p->x15C != -1 ? p->x15C : 0x184;
        p->q190 = D_002A2DD0;
        break;
    case 3:
    case 4:
        anim = 0x183;
        p->q190 = D_002C0D60;
        break;
    case 2:
        p->x1CC = D_002A773C;
        ((void (*)(void *))p->vt[9].pf)((char *)p + p->vt[9].d);
        {
            struct LO *r = func_001CB658(p);
            if (r)
                UnlockProxyPlo(r, p);
        }
        break;
    case 5:
        anim = 0x186;
        p->q190 = D_002A2DD0;
        break;
    }
    switch(n){
    case 0:case 4:case 5:
        q=p->x16C;if(q)((void(*)(void*))q->vt[8].pf)((char*)q+q->vt[8].d);
        break;
    default:
        q=p->x16C;if(q)((void(*)(void*))q->vt[9].pf)((char*)q+q->vt[9].d);
        break;
    }
    p->x150 = n;
    p->x154 = D_002DDED0.t;
    if (anim != -1) {
        if (!p->x168) {
            p->x168 = func_001F0380(p->x164, p, anim, 0);
            ((void (*)(void *))p->x168->vt[1].pf)((char *)p->x168 + p->x168->vt[1].d);
            if (p->x160 > 0.0f)
                func_001F10F0(p->x168, p->x160);
        } else {
            func_001F0660(p->x168, anim);
        }
    } else if (p->x168) {
        func_001F0598(p->x168);
        ((void (*)(void *))p->x168->vt[2].pf)((char *)p->x168 + p->x168->vt[2].d);
        p->x168 = 0;
    }
}
