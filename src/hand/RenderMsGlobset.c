struct S { char pad[0x80]; char x80[0x48]; int fc8; };
struct B { char ab[0x410]; };
void func_001D57A8(struct B *pb);
void CullSceneRenderList(struct B *pb, void *p);
void func_00168638(void *p, int a, int b, float f, float g);
void RenderMsGlobset(struct S *p)
{
    if (p->fc8) {
        struct B b;
        func_001D57A8(&b);
        CullSceneRenderList(&b, p->x80);
    } else {
        func_00168638(p->x80, 0, 0, 1.0f, 1.0f);
    }
}
