class OBase { public: int a, b, c; virtual void V1(); virtual void V2(); virtual void V3(); virtual void V4(); virtual void V5();
 virtual void V6(); virtual void V7(); virtual void V8(); virtual void V9(); };
class O : public OBase { public: char pad[0x54 - 0x10]; unsigned int x54; char p2[8]; float g60; float g64; int p68; int n6C; };
struct S { char pad[0x3E60]; O *o; };
extern "C" O *PloNew__F3CIDP2SO3OIDi(int a, void *p, int b, int c);
extern "C" void func_0020E338(S *p)
{
    p->o = PloNew__F3CIDP2SO3OIDi(0x74, 0, -1, -1);
    p->o->g60 = 0.1f;
    p->o->g64 = 0.25f;
    p->o->x54 = 0x80200000;
    p->o->n6C = 1;
    p->o->V9();
}
