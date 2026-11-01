typedef int ti128 __attribute__((mode(TI)));
struct S { char pad[0x10b8]; int f10b8; char pad2[0x1320 - 0x10bc]; char a1320[0xc0]; char a13e0[0x100]; char a14e0[0x10];
    char a14f0[0x100]; char a15f0[0x10]; char a1600[0x110]; char a1710[0x10]; };
extern ti128 D_002BC920;
void PostLoadStepplayer(struct S *);
int PxfFindSo(struct S *, int, int);
void func_001A0300(struct S *, void *, void *);
void PostJtLoadGait(void *);
void func_00136D28(void *, struct S *, ti128, float, float);
void func_001426E8(struct S *, int);
void SetBentleyState(struct S *, int);
void PostBentleyLoad(struct S *p) {
    PostLoadStepplayer(p);
    p->f10b8 = PxfFindSo(p, 0xb, 1);
    func_001A0300(p, p->a13e0, p->a14e0);
    func_001A0300(p, p->a14f0, p->a15f0);
    PostJtLoadGait(p->a1320);
    PostJtLoadGait(p->a1600);
    func_00136D28(p->a1710, p, D_002BC920, -50.0f, 50.0f);
    func_001426E8(p, 0x16);
    SetBentleyState(p, 0);
}
